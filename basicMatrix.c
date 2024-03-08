#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define R1 1024
#define C1 1024
#define R2 1024
#define C2 1024

/* Basic O(N^3) program to compare time savings
   when parallelizing matrix mult */
int main(int argc, char* argv[])
{
   clock_t begin = clock();
   srand(time(NULL));

   int (*m1)[C1] = malloc(R1 * sizeof(int[C1]));
   int (*m2)[C2] = malloc(R2 * sizeof(int[C2]));
   int (*result)[C2] = malloc(R1 * sizeof(int[C1]));

   int backwards = R1 * C1;
      for (int i = 0; i < R1; i++)
      {
         for (int j = 0; j < C1; j++)
         {
            m1[i][j] = rand() % 10;
            m2[i][j] = rand() % 10;
         }
      }

   // do multiplication here
	for (int i = 0; i < R1; i++)
	{
      for (int j = 0; j < C2; j++)
      {
         result[i][j] = 0;

			for (int k = 0; k < R2; k++)
			{
            result[i][j] += m1[i][k] * m2[k][j];
			}

//      printf("%d\t", result[i][j]);

      }
//	printf("\n");
	}
   clock_t end = clock();

   double timeEx = (double)(end - begin) / CLOCKS_PER_SEC;

   printf("Time spent executing in sec: %f \n", timeEx);

   free(m1);
   free(m2);
   free(result);

   return 0;
}
