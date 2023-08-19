#!/bin/bash
#SBATCH --no-requeue
#SBATCH --job-name="size_scal"
#SBATCH --partition=THIN
#SBATCH -N 1
#SBATCH -n 12
#SBATCH --exclusive
#SBATCH --time=2:00:00
#SBATCH --output="summary.out"

echo Memory allocated
echo
echo Loading modules
module load architecture/Intel
module load mkl
module load openBLAS/0.3.23-omp
echo modules loaded

export LD_LIBRARY_PATH=/u/dssc/ldavin00/myblis/lib:$LD_LIBRARY_PATH

echo
echo compile files
make double
make float
echo files compiled

ncores=12
node=THIN
alloc=close

### setting number of cores and their topology
export OMP_PLACES=cores
export OMP_PROC_BIND=$alloc
export OMP_NUM_THREADS=$ncores

CPUS_PER_TASK=12


#execute requiring 12 cpus per task

echo run files
for size in $(seq 2000 500 20000)
do
	srun -n1 --cpus-per-task=$CPUS_PER_TASK ./DOUBLE/gemm_mkl.exe $size $size $size
	srun -n1 --cpus-per-task=$CPUS_PER_TASK ./DOUBLE/gemm_openblas.exe $size $size $size
	srun -n1 --cpus-per-task=$CPUS_PER_TASK ./DOUBLE/gemm_blis.exe $size $size $size
done

echo files run

make clean
module purge

echo program terminated
