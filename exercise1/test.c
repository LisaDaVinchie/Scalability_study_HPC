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
      // fname = (char*)malloc(50);
      // sprintf(fname, "%s", optarg );
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

  if(action == INIT){
    printf("INIT\n");
  }

  else if(action == RUN){
    printf("RUN\n");
  }

  else{
    printf("Invalid value for variable \"action\"\n");
  }
  return 0;
}
