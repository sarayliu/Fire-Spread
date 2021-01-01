//
// Sara Liu, 9 September 2019
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//
#define M 30
#define N 40
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
void main()
{
   long long int rseed ;
   //
   rseed = 1444611 ;
   //
   srand( rseed );
   //
   printf("p\tAverage normalized burnout time\n");
   for(double deltaP = 0; deltaP < 1.01; deltaP += 0.01)
   {
      totalNormal = 0;
      for(int t = 0; t < 100; t++)
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
      printf("%f\t%f\n", deltaP, totalNormal/100.0);
   //       printf("Average normalized burnout count for p=%f: %f\n", deltaP, totalNormal/100.0);
   }
   //
   printf( "%lli\n" , rseed ) ; 
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
   if( numIgnited > M*N) printf( "uhoh ********************\n");
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