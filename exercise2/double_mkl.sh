#!/bin/bash
#SBATCH --nodes=12
#SBATCH --ntasks-per-node=128
#SBATCH --partition=THIN
#SBATCH --time=1:0:0

module load architecture/AMD
module load mkl
module load openBLAS/0.3.21-omp 

srun -n1 make double

srun -n1 --cpus-per-task=128 ./gemm_mkl.exe 2000 2000 2000

srun -n1 --cpus-per-task=128 ./gemm_openblas.exe 2000 2000 2000
