#include <mpi.h>
#include <stdio.h>
 /* A two-dimensional torus of 12 processes in a 4x3 grid */
int main(int argc, char *argv[])
{
    int rank, rank_comm, size;
    MPI_Comm comm;
    int dim[2], period[2], reorder;
    int coord[2], id;

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
    if (rank_comm == 5)
    {
        MPI_Cart_coords(comm, rank, 2, coord);
        printf("Rank_comm %d coordinates are (%d, %d)\n", rank_comm, coord[0], coord[1]);
    }
    if(rank == 0)
    {
        coord[0]=3; coord[1]=1;
        MPI_Cart_rank(comm, coord, &id);
        printf("The processor at position (%d, %d) has rank %d\n", coord[0], coord[1], id);
    }
    MPI_Comm_free(&comm);
    MPI_Finalize();
    return 0;
}
