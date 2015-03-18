#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include "stack.h"

#define EPSILON 1e-3
#define F(arg)  cosh(arg)*cosh(arg)*cosh(arg)*cosh(arg)
#define A 0.0
#define B 5.0

#define SLEEPTIME 1

int *tasks_per_process;

double farmer(int);

void worker(int);

int main(int argc, char **argv ) {
    int i, myid, numprocs;
    double area, a, b;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    
    if(numprocs < 2) {
        fprintf(stderr, "ERROR: Must have at least 2 processes to run\n");
        MPI_Finalize();
        exit(1);
    }
    
    
    // Initialization
    
    if (myid == 0) { // Farmer
        // init counters
        tasks_per_process = (int *) malloc(sizeof(int)*(numprocs));
        for (i=0; i<numprocs; i++) {
            tasks_per_process[i]=0;
        }
    }
    
    // Main functionality
    
    if (myid == 0) { // Farmer
        area = farmer(numprocs);
    } else { //Workers
        worker(myid);
    }
    
    
    // Output results
    
    if(myid == 0) {
        fprintf(stdout, "Area=%lf\n", area);
        fprintf(stdout, "\nTasks Per Process\n");
        for (i=0; i<numprocs; i++) {
            fprintf(stdout, "%d\t", i);
        }
        fprintf(stdout, "\n");
        for (i=0; i<numprocs; i++) {
            fprintf(stdout, "%d\t", tasks_per_process[i]);
        }
        fprintf(stdout, "\n");
        free(tasks_per_process);
    }
    MPI_Finalize();
    return 0;
}

double farmer(int numprocs)
{
    double temp[2];
    stack *bag = new_stack();
    int size = 0;
    int idle = numprocs;
    int tag , who;
    double total  = 0.0;
    MPI_Status status;
    
    // initialize the bag
    double data[2];
    data[0] = A;
    data[1] = B;

    tasks_per_process = (int *)malloc(sizeof(int)*numprocs);
    
    push(data, bag);
    MPI_Ssend(pop(bag), 2, MPI_DOUBLE,
              1, 0, MPI_COMM_WORLD);
    tasks_per_process[1] ++;
    idle--;
    
    
    
    
    while (is_empty(bag) != 1 && idle != numprocs)
    {
        for (int i=0; i<numprocs; i++)
        {
            MPI_Recv(&temp, 2, MPI_DOUBLE, MPI_ANY_SOURCE,
                     MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            who = status.MPI_SOURCE;
            tag = status.MPI_TAG;
            
            // process
            // if tag == 1 -> result
            if (tag == 1)
            {
                total += temp[0];
                idle++;
            }
            else if (tag == 0)
            {
                push(temp, bag);
                MPI_Ssend(pop(bag), 2, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
                tasks_per_process[i]++;
                idle--;
            }
            
            //todo how to find which worker is idle ???????????
            
            
        }
    }
    
    // call workers with no_more_tag ... ?
    

    return total;
}

void worker(int mypid) {
    // You must complete this function
    double data[2];
    MPI_Status status;
    
    
    
}
