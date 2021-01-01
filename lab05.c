//
// Sara Liu, 4 October 2019
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//
#define M 180
#define N 240
// #define M 360
// #define N 480
// #define M 720
// #define N 960
// #define M 1440
// #define N 1920
#include "mpi.h"
//
char grid[M][N] ;
int steps = 0;
int x[M * N];
int y[M * N];
int numIgnited = 0;
int burnout = 0;
int newX[M * N], newY[M * N];
double p;
double totalNormal = 0;
//
double r()
{
   return rand() * 1.0 / RAND_MAX ;
}
//
void fill( double ) ;
void show() ;
void ignite();
//
int main( int argc , char* argv[] )
{
   //
   // MPI variables
   //
   int        size    ; // same
   int        rank    ; // different
   //
   MPI_Status status  ;
   int        tag = 0 ; // same!
   //
   // other variables
   //
   int        k , j  ;
   double     result[101] ;
   //
   // boilerplate
   //
   MPI_Init(      &argc          , &argv ) ;
   MPI_Comm_size( MPI_COMM_WORLD , &size ) ;
   MPI_Comm_rank( MPI_COMM_WORLD , &rank ) ;
   //
   srand( rank ) ; // different!
   printf("M = %i, N = %i\n", M, N);
   printf("p\tAverage normalized burnout time\n");
   //
   // manager has rank = 0
   //
   if( rank == 0 )
   {
      printf( "waiting...\n" ) ;
      //
   //      for(double deltaP = 0; deltaP < 1.01; deltaP += 0.01)
      {
         double totalAvg[101];
         for(int i = 0; i < 101; i++)
         {
            totalAvg[i] = 0;
         }
         for( k = 1 ; k < size; k++ )
         {
            MPI_Recv( result , 101 , MPI_DOUBLE , k , tag , MPI_COMM_WORLD , &status ) ;
         //
            // printf("*** %d\n",k); 
         //             printf( "%d %20.16f\n" , size , result ) ;
            for(int i = 0; i < 101; i++)
            {
               totalAvg[i] += result[i];
            }
         }
      //
         for(int i = 0; i < 101; i++)
         {
            totalAvg[i] = totalAvg[i]/(size - 1);
            printf("%f\t%f\n", i/100.0, totalAvg[i]);
         }
      }
   }
   //
   // workers have rank > 0
   //
   else
   {
      int counter = 0;
      for(double deltaP = 0; deltaP < 1.005; deltaP += 0.01)
      {
         totalNormal = 0;
         for(int t = 0; t < 256/(size - 1); t++)
         {
            numIgnited = 0;
            burnout = 0;
            steps = 0; 
            fill( deltaP ) ;
         //
         // show(); 
            for (int r = 0; r < M; r++)
            {
               if(grid[r][0] == 'T')
               {
                  grid[r][0] = 'I';
                  x[numIgnited] = r;
                  y[numIgnited] = 0;
                  numIgnited++;
                  if(steps == 0)
                  {
                     steps++;
                  }
               }
            }
         //    printf("Ignited first column: \n");
         //    show();
            while (burnout == 0)
            {
               ignite();
            }
         //
         // printf("Burnout:\n");
         //          show();
         //          printf("Total number of steps for p=%f: %i\n", p, steps); 
         //          printf("Normalized final count for p=%f: %f\n", p, steps * 1.0/N);
            totalNormal += steps * 1.0/N;
         }
         result[counter++] = totalNormal/(256/(size - 1));
      }
      // printf("*** %d %d\n",counter,rank);
      MPI_Send( result , 101 , MPI_DOUBLE , 0 , tag , MPI_COMM_WORLD ) ;
   //
   }
   // boilerplate
   //
   MPI_Finalize() ;
   //
   return 0;
} 
//
void fill( double p )
{
   int j , k ;
   //
   for( j = 0 ; j < M ; j++ )
   {
      for( k = 0 ; k < N ; k++ )
      {
         if( r() < p )         
         {
            grid[j][k] = 'T' ; // on
         }
         else
         {
            grid[j][k] = ' ' ; // off
         }
      }
   }
}

void ignite()
{
   int didSpread = 0;
   int newNumIgnited = 0;
   //
   for( int i = 0 ; i < numIgnited ; i++ )
   {
      int xPos = x[i];
      int yPos = y[i];
      // printf("%i\n", xPos);
   //       printf("%i\n", yPos);
      if(xPos - 1 >= 0 && grid[xPos - 1][yPos] == 'T')
      {
         grid[xPos - 1][yPos] = 'I';
         didSpread = 1;
         newX[newNumIgnited] = xPos - 1;
         newY[newNumIgnited] = yPos;
         newNumIgnited++;
      }
      if(yPos - 1 >= 0 && grid[xPos][yPos - 1] == 'T')
      {
         grid[xPos][yPos - 1] = 'I';
         didSpread = 1;
         newX[newNumIgnited] = xPos;
         newY[newNumIgnited] = yPos - 1;
         newNumIgnited++;
      }
      if(yPos + 1 < N && grid[xPos][yPos + 1] == 'T')
      {
         grid[xPos][yPos + 1] = 'I';
         didSpread = 1;
         newX[newNumIgnited] = xPos;
         newY[newNumIgnited] = yPos + 1;
         newNumIgnited++;
      } 
      if(xPos + 1 < M && grid[xPos + 1][yPos] == 'T')
      {
         grid[xPos + 1][yPos] = 'I';
         didSpread = 1;
         newX[newNumIgnited] = xPos + 1;
         newY[newNumIgnited] = yPos;
         newNumIgnited++;
      }
      grid[xPos][yPos] = ' ';
   }
   if(didSpread == 1)
   {
      steps++;
   }
   else
   {
      burnout = 1;
   }
   for(int assign = 0; assign < newNumIgnited; assign++)
   {
      x[assign] = newX[assign];
      y[assign] = newY[assign];
   }
   // show();
//    printf("Number of steps: %i\n", steps);
   numIgnited = newNumIgnited;
}
//
void show()
{
   int j , k ;
   //
   for( j = 0 ; j < M ; j++ )
   {
      for( k = 0 ; k < N ; k++ )
      {
         printf( "%c" , grid[j][k] ) ;
      }
      printf( "\n" ) ;
   }
}
//
// end of file
//