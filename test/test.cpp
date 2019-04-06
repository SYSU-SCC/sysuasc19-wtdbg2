#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#define MAX_PROCESSES 10

int main( int argc, char **argv )
{
    int rank, size, i,j;
    int errors=0;
    int participants;

    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );

    int *table= (int*)malloc(2*size*sizeof(int));
    int *displs= (int*)malloc(size*sizeof(int));
    int *rec= (int*)malloc(size*sizeof(int));
    for(int c1=0;c1<size;c1++){
        displs[c1]=c1*2;
        rec[c1]=2;
    }
    /* Determine what rows are my responsibility */
    /* Paint my rows my color */
    table[rank] = rank + 1;
    int res[2]={0};
    res[0]=table[rank];
    res[1]=table[rank]*11;
    /* Everybody gets the gathered table */
    MPI_Allgatherv(res, 2, MPI_INT, 
                        table, rec, displs, MPI_INT, MPI_COMM_WORLD);

    MPI_Finalize();
    if(rank==0){
        for(j=0;j<2;j++){
            for (i=0; i<size ;i++){
                printf("%10d ",table[i+j*size]);
            }
            printf("\n");
        }
    }
    free(table);
    free(displs);
    free(rec);
    return errors;
}