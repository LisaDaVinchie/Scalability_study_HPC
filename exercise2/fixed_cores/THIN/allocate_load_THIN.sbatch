#!/bin/bash

#SBATCH --job-name=myjob
#SBATCH --nodes=1
#SBATCH --partition=THIN
#SBATCH --tasks-per-node=1
#SBATCH --cpus-per-task=12
#SBATCH --time=2:00:00

# srun -n1 ./configure --enable-cblas --enable-threading=openmp --prefix=/u/dssc/ldavin00/myblis auto
# srun  -n 1 --cpus-per-task=12 make -j 12
# make install

#salloc -n 12 -N1 -p THIN --time=2:0:0

module load architecture/Intel
module load mkl
module load openBLAS/0.3.23-omp
