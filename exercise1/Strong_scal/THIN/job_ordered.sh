#!/bin/bash
#SBATCH --no-requeue
#SBATCH --job-name="Strong_scal"
#SBATCH --partition=THIN
#SBATCH --nodes=2
#SBATCH --ntasks=4
#SBATCH --ntasks-per-node=2
#SBATCH --cpus-per-task=1
#SBATCH --exclusive
#SBATCH --time=02:00:00
#SBATCH --output="summary.out"

echo Load Modules

module load architecture/Intel
module load intelMPI/2021.7.1

echo set topology

data_folder=$(pwd)
program_folder=../../program
echo data folder is $data_folder

filename="$program_folder/game_of_life_parallel_v3.c"
execname=game_of_life_parallel_v3_1.exe
libname="$program_folder/parallel_lib_v3_1.c"
imagename="playground"

matrix_dim_min=10000
matrix_dim_step=5000
matrix_dim_max=20000

n_steps_min=50
n_steps_step=50
n_steps_max=150

max_threads=4
threads_per_socket=1
max_procs=4
max_nodes=2

export OMP_PLACES=cores
export OMP_PROC_BIND=close
# export MPI_PLACES=sockets
# export MPI_PROC_BIND=close

echo compile c file
mpicc -fopenmp -DTIME -o $execname $filename $libname
echo file compiled, executable is $execname


echo set maximum number of threads

for matrix_dim in $(seq $matrix_dim_min $matrix_dim_step $matrix_dim_max)
do
    # echo initialising playground for matrix dimension $matrix_dim
    # export OMP_NUM_THREADS=$((threads_per_socket*max_procs))
    # mpirun -np $max_procs --map-by socket ./$execname -i -k $matrix_dim -f "${imagename}_${matrix_dim}.pgm"

    for n_procs in $(seq 1 1 $max_procs)
    do
        # export OMP_NUM_THREADS=$((threads_per_socket*n_procs))
        mpirun -np $n_procs --map-by socket ./$execname -r -f "${imagename}_${matrix_dim}.pgm" -e 0 -n $n_steps_min -s $n_steps_min
    done
done

echo removing modules
module purge
echo modules removed
echo removing executable
rm -rf $execname
echo executable removed