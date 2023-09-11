#!/bin/bash
#SBATCH --no-requeue
#SBATCH --job-name="OpenMP_scal"
#SBATCH --partition=THIN
#SBATCH -N 1
#SBATCH -n 24
#SBATCH --exclusive
#SBATCH --time=00:30:00
#SBATCH --output="summary.out"

echo Load Modules

module load architecture/Intel
module load openMPI/4.1.5/gnu/12.2.1
#module load intelMPI/2021.7.1

echo set topology
alloc=close

export OMP_PLACES=cores
export OMP_PROC_BIND=$alloc

data_folder=$(pwd)
program_folder=../../program
echo data folder is $data_folder

echo compile c file
filename="$program_folder/game_of_life_parallel.c"
execname=game_of_life_parallel.exe
libname="$program_folder/parallel_lib.c"
headname="$program_folder/parallel_header.h"
imagename="playground.pgm"

mpicc -fopenmp -DTIME -o $execname $filename $libname

echo file compiled, executable is $execname

echo initialise playground
export OMP_NUM_THREADS=12
# echo initialising playground
# mpirun -np 2 --map-by socket ./$execname -i -k 15 -f $imagename
echo running static evolution
mpirun -np 2 --map-by socket ./$execname -r -f $imagename -e 1 -n 5 -s 2
echo program finished

echo removing modules
module purge
echo modules removed
echo removing executable
rm -rf $execname
echo executable removed
