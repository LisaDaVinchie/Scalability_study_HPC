#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>

#include <time.h>
#include <omp.h>
#include <mpi.h>

#include "parallel_header_v2.h"


#include<unistd.h>


// Definitions for the argument getting part
#define INIT 1
#define RUN  2

#define ORDERED 0
#define STATIC  1
#define MAXVAL 1


// Specify default values
int   action = 0;
int   k      = 100;
int   e      = ORDERED;
int   n      = 100;
int   s      = 50;
char *fname  = NULL;

int main ( int argc, char **argv )
{

//Get the arguments, copy of Tornatore's "get_args.c" program
  char *optstring = "irk:e:f:n:s:";

  // printf("Get arguments\n");
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

  //printf("Arguments are: \naction = %d\nk = %d\ne = %d\nf = %s\nn_steps = %d\nn_dump = %d\n", action, k, e, fname, n, s);

  // initialise playground
  if(action == INIT)
  {
    initialise_playground(k, MAXVAL, fname, argc, argv);
  }
  //End of INIT

  else if(action == RUN){
      
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
      // End of e == ORDERED

      else if(e == STATIC)
      {
        static_evolution_v2(fname, n, s, argc, argv);
      }

      else
      {
        printf("Invalid value for flag \"-e\"\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
      }
 
  }
  // End of action == RUN

  
  else{
    printf("Invalid value for variable \"action\"\n");
    MPI_Abort(MPI_COMM_WORLD, 1);
    return 1;
  }

  if(fname != NULL){
    free(fname);
    fname = NULL;
  }
  
  return 0;
}
// End of prograrm