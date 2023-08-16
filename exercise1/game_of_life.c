#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>


// #include <omp.h>
#include <mpi.h>


//Definitions for the argument getting part
#define INIT 1
#define RUN  2

#define K_DFLT 100

#define ORDERED 0
#define STATIC  1


char fname_deflt[] = "game_of_life.pgm";

int   action = 0;
int   k      = K_DFLT;
int   e      = ORDERED;
int   n      = 10000;
int   s      = 1;
char *fname  = NULL;


//Definitions for the read and write pgm file
// #ifndef 
// #include <stdlib.h>
// #include <stdio.h> 
// #endif


// #define XWIDTH 256
// #define YWIDTH 256
#define MAXVAL 65535


#if ((0x100 & 0xf) == 0x0)
#define I_M_LITTLE_ENDIAN 1
#define swap(mem) (( (mem) & (short int)0xff00) >> 8) +	\
  ( ((mem) & (short int)0x00ff) << 8)
#else
#define I_M_LITTLE_ENDIAN 0
#define swap(mem) (mem)
#endif

#include "./functions/read_pgm_image.c"
#include "./functions/write_pgm_image.c"
// #include "./functions/swap_image.c"
#include "./functions/generate_gradient.c"

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
      fname = (char*)malloc( sizeof(optarg)+1 );
      sprintf(fname, "%s", optarg );
      break;

    case 'n':
      n = atoi(optarg); break;

    case 's':
      s = atoi(optarg); break;

    default :
      printf("argument -%c not known\n", c ); break;
    }
  }
printf("action = %d\nk = %d\ne = %d\nf = %s\nn_steps = %d\nn_dump = %d\n", action, k, e, fname, n, s);
    // if ( fname != NULL )
    //   free ( fname );

//write starting pgm image
int maxval = 255;

void *ptr = generate_gradient( maxval, k, k );

write_pgm_image( ptr, maxval, k, k, fname);


  return 0;
}
