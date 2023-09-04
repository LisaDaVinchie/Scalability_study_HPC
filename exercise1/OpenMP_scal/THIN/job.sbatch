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
echo data folder is $data_folder

cd ../../program
echo moved to $(pwd)

echo compile c file
filename=game_of_life_parallel.c
execname=game_of_life_parallel.exe
libname=parallel_lib.c
headname=parallel_header.h
#filename=test.c
#execname=test.exe

mpicc -fopenmp -o $execname $filename $libname

echo file compiled, executable is $execname

echo initialise playground
export OMP_NUM_THREADS=2
echo initialising playground
mpirun -np 2 --map-by socket ./$execname -i -k 10 -f "$data_folder/playground.pgm"
#echo reading image
#mpirun -np 2 --map-by socket ./$execname -r -k 10 -f "$data_folder/playground.pbm"
#echo image read
module purge
rm -rf $execname
