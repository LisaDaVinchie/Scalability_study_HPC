#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <omp.h>
#include <mpi.h>
#include "parallel_header_v3.h"

#include<unistd.h>

void write_pgm_image_parallel(unsigned char* image, int xwidth, int ywidth, int rows_per_proc, int rank, int maxval, const char* filename)
{
    MPI_File file;
    MPI_File_open(MPI_COMM_WORLD, filename, MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &file);

    // Calculate the local offset within the file
    MPI_Offset local_offset = rank * xwidth * rows_per_proc * sizeof(unsigned char);

    // Set the file view for each process
    MPI_File_set_view(file, local_offset, MPI_UNSIGNED_CHAR, MPI_UNSIGNED_CHAR, "native", MPI_INFO_NULL);

    if (rank == 0) {
        char header[50]; // Adjust the size as needed
        sprintf(header, "P5\n# generated by\n# Lisa Da Vinchie\n%d %d %d\n", xwidth, ywidth, maxval);
        MPI_File_write(file, header, strlen(header), MPI_CHAR, MPI_STATUS_IGNORE);
    }

    // Write the local data to the file
    for(int idx = 0; idx < xwidth * rows_per_proc; idx++)
      image[idx] = image[idx] * 255;

    MPI_Barrier(MPI_COMM_WORLD);

    MPI_File_write(file, image, xwidth * rows_per_proc, MPI_UNSIGNED_CHAR, MPI_STATUS_IGNORE);

    // Close the file
    MPI_File_close(&file);
}

void initialise_playground(int k, int maxval, char* fname, int argc, char **argv)
{

  // initialise_playground(k, maxval, fname, argc, argv);

  int xwidth = k;
  int ywidth = k;

  int rank; // ID of the actual process
  int n_procs; // Total number of porcesses
  int thread_id; // ID of the actual thread 
  int n_threads; // Total number of threads
  int mpi_provided_thread_level;

  int check = -1; // variable used to check the status of MPI functions

  printf("Start MPI environment\n");
  check = MPI_Init_thread(&argc, &argv, MPI_THREAD_FUNNELED, &mpi_provided_thread_level);


  if(check != MPI_SUCCESS){
    printf("A problem arised while initialising MPI\n");
    MPI_Abort(MPI_COMM_WORLD, 1);
  }
  if (mpi_provided_thread_level < MPI_THREAD_FUNNELED )
  {
    printf("A problem arised when asking for MPI_THREAD_FUNNELED levels\n");
    MPI_Abort(MPI_COMM_WORLD, 1);
  }
  // else
  // {
  //   if(rank == 0)
  //     printf("Provided Thread level: %d\n", mpi_provided_thread_level);
  // }


  // Get the rank of the process and the total number of processes 
  MPI_Comm_size(MPI_COMM_WORLD, &n_procs);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);


  // Divide rows between the processes
  int row_per_proc = ywidth / n_procs;
  int remaining_rows_per_proc = ywidth % n_procs;

  if(rank < remaining_rows_per_proc)
    row_per_proc++;
  

  const int cells_per_proc = row_per_proc * xwidth;
  
  // Allocate memory for a partial image
  unsigned char* image = NULL;
  image = (unsigned char*)malloc(cells_per_proc * sizeof(unsigned char));

  if(image == NULL){
    printf("Memory allocation of \"image\" failed\n");
    MPI_Abort(MPI_COMM_WORLD, 1);
  }

  // Create a random playground in parallel
  #pragma omp parallel
  {
    // get the total number of threads
    #pragma omp master
    {
      n_threads = omp_get_num_threads();
      printf("\nSockets: %d\nThreads: %d\n", n_procs, n_threads);
    }

    // Get the number of the actual thread
    thread_id = omp_get_thread_num();
    

    srand(time(NULL) + rank);
    
    // Create a random playground
    #pragma omp for schedule(static)
      for (int idx = 0; idx < cells_per_proc; idx++){
        image[idx] = (unsigned char)((int)rand()%2);
      }
  }

  MPI_Barrier(MPI_COMM_WORLD);

  

  // Save playground
  write_pgm_image_parallel(image, xwidth, ywidth, row_per_proc, rank, maxval, fname);


  if(image != NULL)
  {
    free(image);
    image = NULL;
  }

  // if(full_image != NULL)
  // {
  //   free(full_image);
  //   full_image = NULL;
  // }
  MPI_Finalize();

}