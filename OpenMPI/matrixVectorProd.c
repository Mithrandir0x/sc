#include <math.h>
#include <stdio.h>

#include "mpi.h"

#define NROWS 10
#define NCOLS 10

#define MIN(x,y) ((x)<(y)?(x):(y))

int main(int argc, char **argv)
{
  double ans;
  double A[NROWS][NCOLS];
  double b[NROWS], c[NROWS], buffer[NCOLS];

  int myid, master, numprocs, ierr;
  int i, j, k, numsent, sender;
  int tag;

  MPI_Status status;

  master = 0;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);

  if (myid == master)
  {
    for(i = 0; i < NROWS; i++)
    {
      b[i] = 1;
      for(j = 0; j < NCOLS; j++)
      {
	A[i][j] = 1;
      }
    }

    numsent = 0;
    MPI_Bcast(b, NCOLS, MPI_DOUBLE_PRECISION, master, MPI_COMM_WORLD);

    k = MIN(numprocs-1, NROWS);
    for(i = 0; i < k; i++)
    {
      for(j = 0; j < NCOLS; j++)
	buffer[j] = A[i][j];

      MPI_Send(buffer, NCOLS, MPI_DOUBLE_PRECISION, i+1, i, MPI_COMM_WORLD);
      numsent = numsent + 1;
    }

    for(i = 0; i < NROWS; i++)
    {
      MPI_Recv(&ans, 1, MPI_DOUBLE_PRECISION, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      sender  = status.MPI_SOURCE; // Qui ha enviat el missatge
      tag     = status.MPI_TAG;    // TAG indica la fila
      c[tag] = ans;

      //printf("Master ha rebut el resultat del processador %d per la fila %d: %e\n", sender, tag, ans);

      if (numsent < NROWS)
      {
	for(j = 0; j < NCOLS; j++)
	  buffer[j] = A[numsent][j];

	MPI_Send(buffer, NCOLS, MPI_DOUBLE_PRECISION, sender, numsent, MPI_COMM_WORLD);
	numsent = numsent + 1;
      }
      else
	MPI_Send(&i, 0, MPI_DOUBLE_PRECISION, sender, NROWS, MPI_COMM_WORLD);
    }
  }
  else
  {
    MPI_Bcast(b, NCOLS, MPI_DOUBLE_PRECISION, master, MPI_COMM_WORLD);

    if (myid <= NROWS)
    {
      while (1)
      {
	MPI_Recv(buffer, NCOLS, MPI_DOUBLE_PRECISION, master, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	tag = status.MPI_TAG; // TAG indica la fila

	if (tag == NROWS)
	  break;

	// printf("Processador %d processa fila %d\n", myid, tag);

        ans = 0.0;
	for(j = 0; j < NCOLS; j++)
	  ans = ans + buffer[j] * b[j];

	MPI_Send(&ans, 1, MPI_DOUBLE_PRECISION, master, tag, MPI_COMM_WORLD);
      }
    }
  }

  MPI_Finalize();
  return 0;
}
