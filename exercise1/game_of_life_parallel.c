#include "game_of_life_header.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>

#include <time.h>
#include <omp.h>
#include <mpi.h>

// Definitions for the argument getting part
#define INIT 1
#define RUN  2

#define K_DFLT 100

#define ORDERED 0
#define STATIC  1


// char fname_deflt[] = "game_of_life.pgm";

int   action = INIT;
int   k      = K_DFLT;
int   e      = ORDERED;
int   n      = 10000;
int   s      = 1;
char *fname  = NULL;

#define MAXVAL 1


int main ( int argc, char **argv )
{

//Get the arguments, copy of Tornatore's "get_args.c" program
  char *optstring = "irk:e:f:n:s:";

  printf("Get arguments\n");
  int c;
  while ((c = getopt(argc, argv, optstring)) != -1) {
    switch(c) {
      
    case 'i':
      action = INIT; break;
      
    case 'r':
      action = RUN; break;
      
    case 'k':
      k = atoi(optarg); break;

    case 'e':
      e = atoi(optarg); break;

    case 'f':
      fname = (char*)malloc(50);
      sprintf(fname, "%s", optarg );
      break;

    case 'n': //number of steps to be calculated
      n = atoi(optarg); break;

    case 's': //every how many steps a dump is saved
      s = atoi(optarg);break;

    default :
      printf("argument -%c not known\n", c ); break;
    }
  }
  printf("Arguments are: \naction = %d\nk = %d\ne = %d\nf = %s\nn_steps = %d\nn_dump = %d\n", action, k, e, fname, n, s);

  int xwidth = k;
  int ywidth = k;
  int maxval = MAXVAL;

  int rank, n_procs;
  int mpi_provided_thread_level;

  MPI_Init_thread( &argc, &argv, MPI_THREAD_FUNNELED, &mpi_provided_thread_level);
  // MPI_THREAD_FUNNELED: The process may be multi-threaded, but only the main
  // thread will make MPI calls (funneled through the main thread).

  // &mpi_provided_thread_level: This is an output parameter.
  // After calling MPI_Init_thread, this variable will hold the actual level
  // of thread support provided by the MPI library.


  printf("Provided Thread level: %d\n", mpi_provided_thread_level);

  if ( mpi_provided_thread_level < MPI_THREAD_FUNNELED ){
    printf("A problem arised when asking for MPI_THREAD_FUNNELED level\n");
    MPI_Finalize();
    exit( 1 );
  } 

  MPI_Status status;
  // Declare variable "status" of type "MPI_Status" to hold info about
  // the outcome of a communication


  // Get the rank of the process and the total number of processes 
  MPI_Comm_size(MPI_COMM_WORLD, &n_procs);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  printf("Rank = %d, size = %d\n", rank, n_procs);


  // Get the number of OpenMP threads
  int n_threads = 0;

  #pragma omp parallel
  {
    #pragma omp master
    {
      n_threads = omp_get_num_threads();
      printf("OpenMP threads: %d\n", n_threads);
    }
  }

  // Get the number of threads per process
  // int threads_per_procs = 0;
  #pragma omp parallel
  {
    int threads_per_procs = omp_get_num_threads();
    printf("Threads per process: %d\n", threads_per_procs); 
  }

  

  // char* image = NULL;

  // initialise playground
  if(action == INIT){

    // distribute rows between MPI processes

    int row_per_proc = xwidth / (n_procs - 1); // rows for each process
    int remaining_rows = xwidth % n_procs; // remaining rows

    // if (rank < remaining_rows){
    //   row_per_proc++;
    // }

    int n_cells = row_per_proc * ywidth;


    char* image = NULL;
    image = (char*)malloc(n_cells * sizeof(char));

    if(image == NULL){
      printf("Memory allocation of \"image\" failed\n");
      free(fname);
      return 1;
    }

    #pragma omp parallel
    {
      int thread_id = omp_get_num_threads();
      
      int idx = 0;
      srand(time(NULL));
      
      printf("\nInitial playground, %d part\n", rank);
      #pragma omp for schedule(static, row_per_proc)
        // random_playground(image, row_per_proc, ywidth);
        for (int y = 0; y < ywidth; y++){
            for (int x = 0; x < row_per_proc; x++){
                image[idx] = (char)((int)rand()%2);
                printf("%d ", (int)image[idx]);
                idx++;
            }
            printf("\n");
          }
    }

    //write_pgm_image(image, maxval, xwidth, ywidth, fname);

  }

  else if(action == RUN){
    char* image = NULL;
    // int maxval, xsize, ysize;
    // image = (char*)malloc(xwidth * ywidth * sizeof(char));
    printf("About to read playground\n");
    // char *source_image = "snapshots_static/initial_conditions.pgm";
    read_pgm_image(&image, &maxval, &xwidth, &ywidth, fname);
    printf("Playground read\n");

    int idx = 0;
    for (int y = 0; y < ywidth; y++){
      for (int x = 0; x < xwidth; x++){
          idx++;
      }
    } 
    if(e == ORDERED){
      ordered_evolution(image, xwidth, ywidth, n, s, "snapshots_ordered/snap");
    }
    else if(e == STATIC){
      static_evolution(image, xwidth, ywidth, n, s, "snapshots_static/snap");
    }
    else{
      printf("Invalid value for flag \"-e\"\n");
      free(fname);
      return 1;
    }
   free(image); 
  }

  else{
    printf("Invalid value for variable \"action\"\n");
    free(fname);
    return 1;
  }
  MPI_Finalize();

  free(fname);
  return 0;
}
