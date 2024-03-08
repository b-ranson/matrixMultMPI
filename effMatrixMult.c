#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Current size runs good at 32/64 procs
#define R1 2048
#define C1 2048 // make sure equal with R2
#define R2 2048 // make sure equal with C1
#define C2 2048

int main(int argc, char* argv[])
{
   srand(time(NULL));

   // init all MPI Processes
   int myrank, size;
   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

   double startTime, endTime;
   startTime = MPI_Wtime();

   int (*m1)[C1] = malloc(R1 * sizeof(int[C1]));
   int (*m2)[C2] = malloc(R2 * sizeof(int[C2]));
   int (*result)[R2] = malloc(C1 * sizeof(int[R2]));

   // how many rows of matrix A will each proc have
   int localRows = R1 / size;

   // crash if matrix not evenly divisible by numprocs
   if ( R1 % size != 0)
   {
      printf("Invalid number of procs");
      MPI_Abort(MPI_COMM_WORLD, 1);
   }

   // root proc fills starting matricies with random ints 0-9
   // can comment this out but good to show prog actually works
   if (myrank == 0)
   {
      int backwards = R1 * C1;
      for (int i = 0; i < R1; i++)
      {
         for (int j = 0; j < C1; j++)
         {
            m1[i][j] = rand() % 10;
            m2[i][j] = rand() % 10;
         }
      }
   }

// remove comment to print matricies 1 and 2 (bad for big arrays)
/*
   if (myrank == 0)
   {
      printf("Array 1 \n");
      int backwards = R1 * C1;
      for (int i = 0; i < R1; i++)
      {
         for (int j = 0; j < C1; j++)
         {

            printf("%d ", m1[i][j]);
         }
         printf("\n");
      }
   }

   if (myrank == 0)
   {
      printf("Array 2 \n");
      int backwards = R1 * C1;
      for (int i = 0; i < R1; i++)
      {
         for (int j = 0; j < C1; j++)
         {

            printf("%d ", m2[i][j]);
         }
         printf("\n");
      }
   }
*/
   // 1D array to hold respective rows of M1 to be calculated
   int* localA = malloc(localRows * C1 * sizeof(int));

   // send respective rows to each proc and entire m2 array to all procs
   // prob a better way to do this
   MPI_Scatter(&m1[0][0], localRows * C1, MPI_INT, localA, localRows * C1, MPI_INT, 0, MPI_COMM_WORLD);
   MPI_Bcast(&m2[0][0], R2 * C2, MPI_INT, 0, MPI_COMM_WORLD);

   int localResult[localRows][C2];

   // calculate multiplication
   for (int i = 0; i < localRows; i++)
   {
      for (int j = 0; j < C2; j++)
      {
         localResult[i][j] = 0;
         for (int k = 0; k < C2; k++)
         {
            localResult[i][j] += localA[i * C2 + k] * m2[k][j];
         }
      }
   }

   MPI_Gather(localResult, localRows * C1, MPI_INT, result, localRows * C1, MPI_INT, 0, MPI_COMM_WORLD);

// remove comment to print final result matrix (once again, bad for big arrays)
/*
   if (myrank == 0)
   {
      printf("\nMatrix Result of A * B: \n\n");
      for (int i = 0; i < C1; i++)
      {
         for (int j = 0; j < R2; j++)
         {
            printf("%d ", result[i][j]);
         }
         printf("\n");
      }
   }
*/

   endTime = MPI_Wtime();
   if (myrank == 0)
   {
      printf("Elapsed time: %.6f seconds\n", endTime - startTime);
   }

   free(localA);
   free(m1);
   free(m2);
   free(result);
   MPI_Finalize();

   return 0;
}
