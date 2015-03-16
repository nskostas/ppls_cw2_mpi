// A simple MPI "Hello World"
//
// FIRST, READ THE INSTRUCTIONS IN
//
//    http://www.inf.ed.ac.uk/teaching/courses/ppls/mpiHowTo.html
//
// To compile
//
//    [mymachine]:  /usr/lib64/openmpi/bin/mpicc -o hellompi hellompi.c
//
// and to run
//
//    [mymachine]:  /usr/lib64/openmpi/bin/mpirun -c 8 ./hellompi
//
// where the number (here 8) gives the number of processes you want.
//

#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    
    int rank, p;
    
    // Explore the world
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    
    //  Say hello
    printf ("Hello world from process %d of %d\n", rank, p);
    
    MPI_Finalize();
}
