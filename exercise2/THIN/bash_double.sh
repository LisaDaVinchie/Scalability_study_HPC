#!/bin/bash

# requested nodes
#SBATCH --nodes=1

# Tasks per node
#SBATCH --ntasks-per-node=24

# Partition
#SBATCH --partition=THIN

# Time limit: 1h
#SBATCH --time=1:0:0

# Load needen modules
module load architecture/Intel
module load mkl
module load openBLAS/0.3.21-omp

# choose matrix dimesion
matrix_dim = 2000

# run dgemm with double precision
srun -n1 make double

#execute requiring 24 cpus per task
srun -n1 --cpus-per-task=24 ./gemm_mkl.exe $matrix_dim $matrix_dim $matrix_dim


srun -n1 --cpus-per-task=24 ./gemm_openblas.exe $matrix_dim $matrix_dim $matrix_dim


srun -n1 --cpus-per-task=24 ./gemm_blis.exe $matrix_dim $matrix_dim $matrix_dim

