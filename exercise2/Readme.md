# Exercise 2

## Structure of this directory

This directory contains:

- The "gemm.c" file, that is used to call the *gemm* function, measure its performance and save the obtained data into a csv file.
- The *Data_Analysis.ipynb* file, used to analyze the data, and the png graphs produced by the analysis.
- Two directories: *fixed_matrix* and *fixed_cores*, that are used to store the data obtained keeping the matrix size fixed and the number pf cores fixed, respectively.

The two directories have the same structure, that is made of:

- A Makefile, used to compile the *gemm.c* script for all the three libraries (MKL, OpenBlas, Blis)
- Two sbatch files, used to allocate memory, load the libraries, call the makefile, run the executable files and redirect the results in the right directory; one file is for the Thin nodes, while the other is for the Epyc ones.
- Two directories: *THIN* for the results obtained using Thin nodes and *Epyc* for the results obtained using Epyc nodes.

THIN and Epyc are both splitted into the directories:
-*close_cores*: contains the results obtained using *OMP_PROC_BIND=close*, that instructs the execution environment to assign the threads in the team to the places that are close to the place of the parent thread
-*spread cores*: contains the results obtained using *OMP_PROC_BIND=spread*, that instructs the execution environment to spread the threads as evenly as possible among the available places.

Finally, both the directories contain 6 csv files, one for each library (MKL, OpenBlas and Blis) and for each precision (double and float), that are.

A simple scheme of how this directory is made is:

*exercise2*
    gemm.c
    *fixed_matrix*
        EPYC.sbatch
        THIN.sbatch
        Makefile
        *THIN*
            close_cores
                blis_f.csv
                blis_d.csv
                mkl_d.csv
                mkl_f.csv
                openblas_d.csv
                openblas_f.csv
            spread_cores
                blis_f.csv
                blis_d.csv
                mkl_d.csv
                mkl_f.csv
                openblas_d.csv
                openblas_f.csv
        *EPYC*
            close_cores
                blis_f.csv
                blis_d.csv
                mkl_d.csv
                mkl_f.csv
                openblas_d.csv
                openblas_f.csv
            spread_cores
                blis_f.csv
                blis_d.csv
                mkl_d.csv
                mkl_f.csv
                openblas_d.csv
                openblas_f.csv
    *fixed_cores*
        EPYC.sbatch
        THIN.sbatch
        Makefile
        *THIN*
            close_cores
                blis_f.csv
                blis_d.csv
                mkl_d.csv
                mkl_f.csv
                openblas_d.csv
                openblas_f.csv
            spread_cores
                blis_f.csv
                blis_d.csv
                mkl_d.csv
                mkl_f.csv
                openblas_d.csv
                openblas_f.csv
        *EPYC*
            close_cores
               blis_f.csv
                blis_d.csv
                mkl_d.csv
                mkl_f.csv
                openblas_d.csv
                openblas_f.csv
            spread_cores
                blis_f.csv
                blis_d.csv
                mkl_d.csv
                mkl_f.csv
                openblas_d.csv
                openblas_f.csv

## Makefile

The two makefile files are identical and are repeated just for convenience. Their structure can be divided into 3 parts:

1- Declare the correct paths for the three libraries and for gemm.c and take as input the path where the results will be stored.
2- Compile gemm.c for each library, assigning different names to the three executable files that will be given as output; the precision can be selected using the phony targets *double* and *float*.
3- Remove the executable files using the *clean* phony target.

## Sbatch files

In total there are four sbatch files, that must be used depending on which kind of node we want to allocate and on the parameter that we want to keep fixed during the scalability test (number of cores or matrix size). The four files, however, share a common structure:

- Allocate the cores, specifying what kind of node we want to use (THIN or EPYC), how many cores we need at maximum and for how much time
- Load the modules and install the Blis library
- Use *export* to set the number of cores to use, their topology (spread or close)
- Find the correct path to store the results, depending on the node and on the topology of the cores
- Use a loop to run the executable files for different numbers of cores or matrix sizes, taking 5 measures for each configuration
- Unload the modules and delete the executable files

In order to run the file, open the sbatch file corresponding to the node you want to use and change the variable of the value *blis_path* with the path where you want blis to be saved, *blis_makefile* with the path where you downloaded the Blis github repository and assign to the variable *alloc* either the value "close" or the value "spread", depending on the topology you want to use.