#include <mpi.h>
#include <stdio.h>
 /* A two-dimensional torus of 12 processes in a 4x3 grid */
int main(int argc, char *argv[])
{
    int rank, rank_comm, size, source, dest;
    MPI_Comm comm;
    int dim[2], period[2], reorder;
    int coord[2];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size != 12)
    {
        printf("Please run with 12 processes.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    dim[0]=4; dim[1]=3;
    period[0]=1; period[1]=0;
    reorder=1;
    MPI_Cart_create(MPI_COMM_WORLD, 2, dim, period, reorder, &comm);
    MPI_Comm_rank(comm, &rank_comm);

    if (rank_comm == 2)
    {
      MPI_Cart_coords(comm, rank_comm, 2, coord);
      printf("Rank_comm %d coordinates are (%d, %d)\n", rank_comm, coord[0], coord[1]);

      MPI_Cart_shift(comm, 1, 1, &source, &dest);

      if (source == MPI_PROC_NULL)
	printf("Source is null\n");
      else
      {
	MPI_Cart_coords(comm, source, 2, coord);
	printf("Source is %d with coordinates (%d, %d)\n", source, coord[0], coord[1]);
      }

      if (dest == MPI_PROC_NULL)
	printf("Destination is null\n");
      else
      {
	MPI_Cart_coords(comm, dest, 2, coord);
	printf("Destination is %d with coordinates (%d, %d)\n", dest, coord[0], coord[1]);
      }

    }
    MPI_Comm_free(&comm);
    MPI_Finalize();
    return 0;
}
