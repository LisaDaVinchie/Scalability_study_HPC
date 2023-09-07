#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>

#include <time.h>
#include <omp.h>
#include <mpi.h>

#include "parallel_header.h"


// Definitions for the argument getting part
#define INIT 1
#define RUN  2

#define K_DFLT 100

#define ORDERED 0
#define STATIC  1
#define MAXVAL 1


// Specify default values
int   action = 0;
int   k      = K_DFLT;
int   e      = ORDERED;
int   n      = 10000;
int   s      = 1;
char *fname  = NULL;



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
      //If action = INIT it is the name of the playground that will be created
      // if action = RUN it is the name of the playground that will be read
      fname = (char*)malloc(100);
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

  if (fname == NULL){
    printf("No value assigned to f\n");
    return 1;
  }
  if(action == 0){
    printf("No action specified\n");
    return 1;
  }

  printf("Arguments are: \naction = %d\nk = %d\ne = %d\nf = %s\nn_steps = %d\nn_dump = %d\n", action, k, e, fname, n, s);


  int rank, n_procs;
  int mpi_provided_thread_level;

  MPI_Init_thread(&argc, &argv, MPI_THREAD_FUNNELED, &mpi_provided_thread_level);
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

  

  // Note that y = row index, x = col index

  // initialise playground
  if(action == INIT){

    int xwidth = k;
    int ywidth = k;
    int maxval = MAXVAL;

    // distribute rows between MPI processes

    int row_per_proc = xwidth / n_procs; // rows for each process
    int remaining_rows = xwidth % n_procs; // remaining rows

    //If there are spare rows, addign one extra row to each process
    if (remaining_rows > 0){
      row_per_proc++;
    }

    int n_cells = row_per_proc * xwidth;

    
    unsigned char* image = NULL;
    image = (unsigned char*)malloc(n_cells * sizeof(unsigned char));

    if(image == NULL){
      printf("Memory allocation of \"image\" failed\n");
      free(fname);
      return 1;
    }

    // Create a random playground in parallel
    #pragma omp parallel
    {
      int thread_id = omp_get_num_threads();
      
      srand(time(NULL) + rank);
      
      unsigned int seed = (unsigned int)time(NULL); 

      // Create a random playground
      #pragma omp for schedule(static)
        for (int idx = 0; idx < n_cells; idx++){
          image[idx] = (unsigned char)((int)rand()%2);
        }
    }

    // Gather the image parts created by each thread in a single array in the master thread
    unsigned char* full_image = NULL;
    if(rank == 0){
      full_image = (unsigned char*)malloc(xwidth * ywidth * sizeof(unsigned char));

      if(full_image == NULL){
        printf("Memory allocation of \"image\" failed\n");
        free(fname);
        return 1;
      }
    }

    // Make sure that all the processes terminated
    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Gather(image, n_cells, MPI_UNSIGNED_CHAR, full_image, n_cells, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);

    free(image);

    // Print the obtained image
    if(rank == 0){
      int idx = 0;
      for (int x = 0; x < xwidth; x++){
        for(int y = 0; y < ywidth; y++){
          printf("%d ", (int)full_image[idx]);
          idx++;
        }
        printf("\n");
      }

      // Save the obtained image, serial
      write_pgm_image(full_image, xwidth, ywidth, maxval, fname);
      free(full_image);
    }
    MPI_Finalize();
  }

  else if(action == RUN){

    #ifdef TIME
      double start_time;
    #endif
    int xwidth = -1, ywidth = -1, maxval = -1;
    unsigned char* image = NULL;

    // printf("About to read playground\n");

    
    // if (rank == 0){
    //   // Read the header to get the dimensions of the playground and the color maxval
    //   read_header(&xwidth, &ywidth, &maxval, fname);

    //   // Allocate memory to read the playground
    //   image = (unsigned char*)malloc(xwidth * ywidth * sizeof(unsigned char));

    //   // Read the playground and store it in a array
    //   read_pgm_image(image, xwidth, ywidth, maxval, fname);

    //   printf("Playground read\n");

    //   // Print the playground
    //   int idx = 0;
    //   for (int y = 0; y < ywidth; y++){
    //     for (int x = 0; x < xwidth; x++){
    //         printf("%d ", (int)image[idx]);
    //         idx++;
    //     }
    //     printf("\n");
    //   }


    // // distribute rows between MPI processes
    // int row_per_proc = xwidth / n_procs; // rows for each process
    // int remaining_rows = xwidth % n_procs; // remaining rows

    // // If there are spare rows, add one row for each process
    // if (remaining_rows > 0){
    //   row_per_proc++;
    // }

    // printf("Thread %d has %d rows\n", rank, row_per_proc);

    // int n_cells = row_per_proc * ywidth;

    // }

    
    if(e == ORDERED){
      // ordered_evolution(image, xwidth, ywidth, n, s, "snapshots_ordered/snap");
      // char* title = "snapshots_ordered/snap";
      // int snap_idx = -1;
      // // int maxval = 1;

      // #ifdef TIME
      //   MPI_Barrier(MPI_COMM_WORLD);
        
      //   if(rank == 0){

      //   }
      // #endif

      // if(s == 0){
      //   snap_idx = n;
      // }
      // else if(s > 0 && s < n){
      //   snap_idx = s;
      // }
      // else{
      //   printf("Wrong value for \"s\"\n");
      // }

      // // dividing the loop among MPI processes
      
      // #pragma omp parallel for
      // for(int step = 0; step < n; step++){
      //   //ordered evolution
      //   printf("step = %d ", step);
      //   for(int y = 0; y < xwidth; y++){
      //     for (int x = 0; x < ywidth; x++){
      //       printf("    y = %d ", y);
      //       printf("    x = %d\n", x);
      //       //calculate status of cell in [x][y]
      //       unsigned char live_neighbors = count_live_neighbors(image, x, y, xsize, ysize);
      //       if (live_neighbors == 2 || live_neighbors == 3){
      //         image[x + y * xwidth] = 1;
      //       }
      //       else if(live_neighbors < 0 || live_neighbors > 8){
      //         printf("There is an issue with the count of the neighbors that are alive, they cannot be %d\n", (int)live_neighbors);
      //       }
      //       else{
      //         image[x + y * xwidth] = 0;
      //       }
      //     }
      //   }
        

      //   if((step + 1)%snap_idx == 0){
      //     printf("\nTaking snapshot\n");
      //     int idx = 0;
      //       for (int y = 0; y < ywidth; y++){
      //           for (int x = 0; x < xwidth; x++){
      //               printf("%d ", (int)image[idx]);
      //               idx++;
      //           }
      //       printf("\n");
      //     }
      //     char title[50];
      //     snprintf(title, 50, "%s_%d.pbm", destination_folder, step);
      //     printf("%s\n",title);
      //     // dump_idx++;
      //     printf("Writing image\n");
      //     write_pgm_image(image, xwidth, ywidth, maxval, title);
      //     printf("Image written\n");
      //   }
      // }
    }
    else if(e == STATIC){

      printf("About to read playground\n");
      
      unsigned char* original_image = NULL;
      
      if (rank == 0){
        // Read the header to get the dimensions of the playground and the color maxval
        read_header(&xwidth, &ywidth, &maxval, fname);

        printf("xwidt = %d, ywidth = %d\n", xwidth, ywidth);
        // Allocate memory to read the playground
        original_image = (unsigned char*)malloc(xwidth * ywidth * sizeof(unsigned char));

        // Read the playground and store it in a array
        read_pgm_image(original_image, xwidth, ywidth, maxval, fname);

        printf("Playground read\n");

        // Print the playground
        int idx = 0;
        for (int y = 0; y < ywidth; y++){
          for (int x = 0; x < xwidth; x++){
              printf("%d ", (int)original_image[idx]);
              idx++;
          }
          printf("\n");
        }
      }

      #pragma omp barrier

      // MPI_Bcast(original_image, xwidth * ywidth, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);

      #pragma omp barrier
      // distribute rows between MPI processes
      int row_per_proc = ywidth / n_procs; // rows for each process
      int remaining_rows = ywidth % n_procs; // remaining rows

      printf("row_per_proc = %d, remaining rows = %d\n", row_per_proc, remaining_rows);

      // If there are spare rows, add one row for each process
      if (rank < remaining_rows){
        row_per_proc++;
      }

      int n_cells = row_per_proc * xwidth;

      int startrow = rank * row_per_proc;
      int endrow = (rank + 1) * row_per_proc;

        // Allocate partial matrices
        image = (unsigned char*)malloc(row_per_proc * ywidth * sizeof(unsigned char));

        printf("Thread %d has %d rows\n", rank, row_per_proc);
        
        // #pragma omp barrier
      
        if(rank == 0){
          #ifdef TIME
            MPI_Barrier(MPI_COMM_WORLD);

            if(rank == 0){
              start_time = omp_get_wtime();
            }
          #endif
        }

        // #pragma omp barrier

        char* title =  "snap_test";

        int snap_idx = -1;
        int maxval = 1;

        // #pragma omp barrier

        if(rank == 0){
          printf("Define snap index\n");
          if(s == 0){
            snap_idx = n;
          }
          else if(s > 0 && s < n){
            snap_idx = s;
          }
          else{
            printf("Wrong value for \"s\"\n");
          }
        }
        printf("Define snap index\n");
        
        printf("Start cycle\n");
        for(int step = 0; step < n; step++){
          printf("Step %d\n", step);
          #pragma omp parallel
          {
            int thread_id = omp_get_thread_num();

            printf("Upgrade cells\n");
            for(int y = startrow; y < endrow; y++){
              for (int x = 0; x < xwidth; x++){
                //upgrade status of cell (x, y)
                static_upgrade(image, original_image, xwidth, ywidth, x, y);
              }
            }
          }

          // Make the obtained image the starting point for the next cycle
          MPI_Barrier(MPI_COMM_WORLD);

          printf("Gather info\n");
          MPI_Gather(image, n_cells, MPI_UNSIGNED_CHAR, original_image, n_cells, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);

          if(rank == 0){
            // for(int i = 0; i < xwidth * ywidth; i++){
            //   original_image[i] = image[i];
            // }

            if((step + 1)%snap_idx == 0){
              save_snapshot(original_image, xwidth, ywidth, maxval, "snap_test", step);
            }
          }
        }
        printf("Wrote all the snapshots\n");
    }

    else{
        printf("Invalid value for flag \"-e\"\n");
        free(fname);
        return 1;
    }
  
   
   free(image);

   MPI_Finalize();
  }

  else{
    printf("Invalid value for variable \"action\"\n");
    free(fname);
    MPI_Finalize();
    return 1;
  }

  free(fname);
  return 0;
}

