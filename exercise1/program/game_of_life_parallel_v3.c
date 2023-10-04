#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>

#include <time.h>
#include <omp.h>
#include <mpi.h>

#include "parallel_header_v3.h"


#include<unistd.h>


// Definitions for the argument getting part
#define INIT 1
#define RUN  2

#define ORDERED 0
#define STATIC  1
#define MAXVAL 1


// Specify default values
int   action = 0;
int   x      = 100;
int   y      = 100;
int   e      = ORDERED;
int   n      = 100;
int   s      = 50;
char *fname  = NULL;

int main ( int argc, char **argv )
{

//Get the arguments, copy of Tornatore's "get_args.c" program
  char *optstring = "irx:y:e:f:n:s:";

  // printf("Get arguments\n");
  int c;
  while ((c = getopt(argc, argv, optstring)) != -1) {
    switch(c) {
      
    case 'i':
      action = INIT; break;
      
    case 'r':
      action = RUN; break;

    case 'x':
      x = atoi(optarg); break;
    
    case 'y':
      y = atoi(optarg); break;

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
  if((x == 100 || y == 100) && action == INIT)
  {
    printf("Error getting dimensions\n");
    return 1;
  }


  printf("Arguments are: \naction = %d\nx = %d\ny = %d\ne = %d\nf = %s\nn_steps = %d\nn_dump = %d\n", action, x, y, e, fname, n, s);


  if(action == INIT)
  {
    initialise_playground(x, y, MAXVAL, fname, argc, argv);
  }

  else if(action == RUN && e == ORDERED)
  {
    ordered_evolution(fname, n, s, argc, argv);
  }

  else if(action == RUN && e == STATIC)
  {
    static_evolution(fname, n, s, argc, argv);
  }
  
  else
  {
    printf("Invalid value for variable \"action\" or \"e\"\n");
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