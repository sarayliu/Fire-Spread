//
// Sara Liu, 6 September 2019
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//
#define M 30
#define N 40
//
char grid[M][N] ;
int slotOn = 0;
int leftSlotOn = 0;
int nextSlotOn = 0;
int adjacent = 0;
//
double r()
{
   return rand() * 1.0 / RAND_MAX ;
}
//
void fill( double ) ;
void show() ;
//
void main()
{
   long long int rseed ;
   //
   rseed = 1444611 ;
   //
   srand( rseed );
   //
   fill( 0.60 ) ;
   //
   show() ;
   //
   printf( "%lli\n" , rseed ) ; 
   printf("Number of slots turned on: %i\n", slotOn);
   printf("Percent of 1200 total slots turned on: %f%%\n", slotOn/1200.0 * 100);
   printf("Number of slots turned on in left column: %i\n", leftSlotOn);
   printf("Number of slots turned on in next column: %i\n", nextSlotOn);
   printf("Number of on-slots in column #2 adjacent to an on-slot from column #1: %i\n", adjacent);
   
   slotOn = 0;
   for(int i = 0; i < 99; i++)
   {
      fill(0.60);
//       show();
//       printf("\n");
   }
   printf("Average percent of 1200 total slots turned on: %f%%\n", slotOn/(1200.0 * 100) * 100);
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
            slotOn++;
            if(k == 0)
            {
               leftSlotOn++;
            }
            else if(k == 1)
            {
               nextSlotOn++;
               if(grid[j][0] == 'T')
               {
                  adjacent++;
               }
            }
         }
         else
         {
            grid[j][k] = ' ' ; // off
         }
      }
   }
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