#!/bin/bash

# Requested nodes
#SBATCH --nodes=1

# Tasks per node
#SBATCH --ntasks-per-node=24

# Partition
#SBATCH --partition=THIN

# Time limit: 2h
#SBATCH --time=2:0:0

# Load needen modules
module load architecture/Intel
module load mkl
module load openBLAS/0.3.23-omp

# choose matrix dimesion
MATRIX_DIM=2000

# run dgemm with double precision
srun -n1 make double

#execute requiring 24 cpus per task
srun -n1 --cpus-per-task=24 ./gemm_mkl.exe $MATRIX_DIM $MATRIX_DIM $MATRIX_DIM


srun -n1 --cpus-per-task=24 ./gemm_openblas.exe $MATRIX_DIM $MATRIX_DIM $MATRIX_DIM


srun -n1 --cpus-per-task=24 ./gemm_blis.exe $MATRIX_DIM $MATRIX_DIM $MATRIX_DIM

