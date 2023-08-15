#include <stdio.h>
#include <mpi.h>
#include <string.h>
#include <stdlib.h>

// Read arguments
// -i: Initialise a playground
// -r: Run a playground
// -k <num. value>: Playground size
// -e <[0|1]>: Evolution type; 0 = "ordered", 1 = "static"
// -f <string>: the name of the file to be either read or written
// -n <num. value>: Number of steps to be calculated
// -s <num. value>: Every how many steps a dump of the system is saved on a file (0 meaning only at the end)

//evolution type
#define ORDERED 0
#define STATIC 1

//run/initialise

#define INIT 1
#define RUN 2

//minimum size of the playground
#define MINSIZE 100

//default number of steps
#define DFT_STEPS 10000


int main(int argc, char **argv){


int opt;

int action = ORDERED; //action = 1 => initialise, action = 2 => run
int k = MINSIZE; //playground size
int e = INIT; //evolution type
string filename = "playground"; //name of the file
int n_steps = DFT_STEPS; // number of steps
int n_dump; //Every how many steps a dump of the system is saved on a file (0 meaning only at the end)


while(opt = getopt(argc, argv, optstring) != -1){
    switch (opt){
        case 'i':
            action = INIT; //initialise
            break;

        case 'r':
            action = RUN; //run
            break;

        case 'k':
            k = atoi(optstring); //playground size
            if k < MINSIZE{
                printf("Playground size must be greater than %d\n", MINSIZE);
                return 1;
            }
            break;

        case 'e':
            e = atoi(optstring);
            if (e != ORDERED || e != STATIC){
                printf("The value of e must be %d or %d.\n", ORDERED, STATIC);
                return 1;
            }
            break;

        case 'f':
            filename = optarg;
            break;

        case 'n':
            n_steps = atoi(optstring);
            if n_steps < 1{
                printf("The number of steps must be positive\n");
                return 1;
            }
            break;

        case 's':
            n_dump = atoi(optstring);
            break;
        
        default:
            printf("Argument %s not valid\n", opt);
            break;

    }
}

printf("action = %d\nk = %d\ne = %d\nf = %s, n_steps = %d\nn_dump = %d\n", action, k, e, filename, n_steps, n_dump);
return 0;
}
