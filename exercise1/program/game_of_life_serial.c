#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>

#include <time.h>

#include "serial_header.h"
// #include <omp.h>
// #include <mpi.h>


// Definitions for the argument getting part
#define INIT 1
#define RUN  2

#define K_DFLT 100

#define ORDERED 0
#define STATIC  1


// char fname_deflt[] = "game_of_life.pgm";

int   action = 0;
int   k      = K_DFLT;
int   e      = ORDERED;
int   n      = 10000;
int   s      = 1;
char *fname  = NULL;


int main ( int argc, char **argv )
{

//Get the arguments, copy of Tornatore's "get_args.c" program
  int action = 0;
  char *optstring = "irk:e:f:n:s:";

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
      fname = (char*)malloc(sizeof(optarg)+1 );
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
  printf("action = %d\nk = %d\ne = %d\nf = %s\nn_steps = %d\nn_dump = %d\n", action, k, e, fname, n, s);

  // char *destination_folder = "./snapshots"; //Where the results will be saved
  if(action == INIT){

    int xwidth = k;
    int ywidth = k;
    int maxval = 1;

    unsigned char* image = NULL;
    image = (unsigned char*)malloc(xwidth * ywidth * sizeof(unsigned char));

    random_playground(image, xwidth, ywidth);
    write_pgm_image(image, xwidth, ywidth, maxval, fname);

    printf("\nInitial playground\n");
    int idx = 0;
      for (int y = 0; y < ywidth; y++){
          for (int x = 0; x < xwidth; x++){
              printf("%d ", (int)image[idx]);
              idx++;
          }
      printf("\n");
    }

    free(image);

  }

else if(action == RUN){
  
  int xwidth = -1, ywidth = -1, maxval = -1;
  read_header(&xwidth, &ywidth, &maxval, fname);

  // printf("x = %d, y = %d, maxval = %d\n", xwidth, ywidth, maxval);

  unsigned char* image = NULL;
  image = (unsigned char*)malloc(xwidth * ywidth * sizeof(unsigned char));

  read_pgm_image(image, xwidth, ywidth, maxval, fname);


  int idx = 0;

  printf("initial playground\n");
  for (int y = 0; y < ywidth; y++){
    for (int x = 0; x < xwidth; x++){
        printf("%d ", (int)image[idx]);
        idx++;
    }
    printf("\n");
  }

  // unsigned char live_n = count_live_neighbors(image, 0, 0, xwidth, ywidth);

  // printf("Live neighbors: %d\n", (int)live_n);
  printf("\n"); 
  if(e == ORDERED){
    ordered_evolution(image, xwidth, ywidth, n, s, "snap_test");
  }
  else if(e == STATIC){
    // static_evolution(image, xwidth, ywidth, n, s, "snap_test");

    char* title =  "snap_test";
    unsigned char* original_image = NULL;
    original_image = (unsigned char*)malloc(xwidth * ywidth * sizeof(unsigned char));

    int idx = 0;
    for(int x = 0; x < xwidth; x++){
      for(int y = 0; y < ywidth; y++){
        original_image[idx] = image[idx];
        // printf("%d ", (int)original_image[idx]);
        idx++;
      }
      // printf("\n");
    }

    int snap_idx = -1;
    int maxval = 1;
    if(s == 0){
      snap_idx = n;
    }
    else if(s > 0 && s < n){
      snap_idx = s;
    }
    else{
      printf("Wrong value for \"s\"\n");
    }

    for(int step = 0; step < n; step++){
      for(int y = 0; y < ywidth; y++){
        for (int x = 0; x < xwidth; x++){
          //upgrade status of cell (x, y)
          static_upgrade(image, original_image, xwidth, ywidth, x, y);
        }
      }

      // Make the obtained image the starting point for the next cycle

      for(int i = 0; i < xwidth * ywidth; i++){
        original_image[i] = image[i];
      }

      if((step + 1)%snap_idx == 0){
        save_snapshot(original_image, xwidth, ywidth, maxval, "snap_test", step);
      }
    }
    printf("Wrote all the snapshots\n");


  }
  else{
    printf("Invalid value for flag \"-e\"\n");
    return 1;
  }

  free(image);
 
}

else{
  printf("Invalid value for variable \"action\"\n");
  return 1;
}
return 0;
}