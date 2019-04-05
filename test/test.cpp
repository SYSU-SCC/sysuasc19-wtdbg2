#include "mpi.h"  
#include <stdio.h>  

int main(int argc, char* argv[])
{
    int rank, numproces;
    int namelen;
    char processor_name[MPI_MAX_PROCESSOR_NAME];

    fprintf(stderr, "hello world!\n");

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);//获得进程号
    MPI_Comm_size(MPI_COMM_WORLD, &numproces);//返回通信子的进程数

    MPI_Get_processor_name(processor_name, &namelen);
    fprintf(stderr, "hello world! process %d of %d on %s\n", rank, numproces, processor_name);
    MPI_Finalize();
    
    fprintf(stderr, "hello world! process %d of %d on %s\n", rank, numproces, processor_name);
    return 0;
}