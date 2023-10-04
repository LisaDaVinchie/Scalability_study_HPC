#!/bin/bash
#SBATCH --no-requeue
#SBATCH --job-name="OpenMP_scal"
#SBATCH --partition=THIN
#SBATCH -N 1
#SBATCH -n 24
#SBATCH --exclusive
#SBATCH --time=02:00:00
#SBATCH --output="summary.out"

echo Load Modules

module load architecture/Intel
# module load openMPI/4.1.5/gnu/12.2.1
module load intelMPI/2021.7.1

echo set topology

alloc=close

data_folder=$(pwd)
program_folder=../../program
echo data folder is $data_folder

filename="$program_folder/game_of_life_parallel_v2.c"
execname=game_of_life_parallel.exe
libname="$program_folder/parallel_lib.c"
headname="$program_folder/parallel_header.h"
imagename="playground"

matrix_dim_min=10
matrix_dim_step=10
matrix_dim_max=100

n_steps=2
snap_idx=2

max_threads=12
max_processes=1

export OMP_PLACES=cores
export OMP_PROC_BIND=$alloc

echo compile c file
mpicc -fopenmp -DTIME -o $execname $filename $libname
echo file compiled, executable is $execname


echo set maximum number of threads
export OMP_NUM_THREADS=$max_threads


for matrix_dim in $(seq $matrix_dim_min $matrix_dim_step $matrix_dim_max)
do
    echo initialising playground
    mpirun -np $max_processes --map-by socket ./$execname -i -k $matrix_dim -f "${imagename}_${matrix_dim}.pgm"

    for n_threads in $(seq 1 1 $max_threads)
    do
        export OMP_NUM_THREADS=$n_threads
        mpirun -np $max_processes --map-by socket ./$execname -r -f "${imagename}_${matrix_dim}.pgm" -e 1 -n $n_steps -s $snap_idx
    done
done

# mpirun -np $max_processes --map-by socket ./$execname -i -k $matrix_dim_min -f playground.pgm

# for n_threads in $(seq 1 1 $max_threads)
# do
#     export OMP_NUM_THREADS=$n_threads
#     mpirun -np $max_processes --map-by socket ./$execname -r -f playground.pgm -e 1 -n $n_steps -s $snap_idx
# done

echo removing modules
module purge
echo modules removed
echo removing executable
rm -rf $execname
echo executable removed
