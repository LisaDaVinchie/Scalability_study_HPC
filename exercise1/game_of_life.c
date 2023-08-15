#include <stdio.h>
#include <mpi.h>
#include <string.h>

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


int main(int argc, char **argv){


int opt;

int action; //action = 1 => initialise, action = 2 => run
int k; //playground size
int e; //evolution type
string filename; //name of the file
int n_steps; // number of steps
int n_dump; //Every how many steps a dump of the system is saved on a file (0 meaning only at the end)


while(opt = getopt(argc, argv, optstring) != -1){
    switch (opt){
        case 'i':
            action = 1
            break;
        case 'r':
            action = 2
            break;
        case 'k':
            k = atoi(optstring)
            break;
        case 'e':
            action =
            break;
        case 'f':
            action =
            break;
        case 'n':
            action =
            break;
        case 's':
            action =
            break;
        
    }
    {
    case /* constant-expression */:
        /* code */
        break;
    
    default:
        break;
    }

}

}
