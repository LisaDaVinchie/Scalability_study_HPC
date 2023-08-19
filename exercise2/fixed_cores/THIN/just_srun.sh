#!/bin/bash

MATRIX_DIM=2000
#execute requiring 12 cpus per task
srun -n1 --cpus-per-task=12 ./DOUBLE/gemm_mkl.exe $MATRIX_DIM $MATRIX_DIM $MATRIX_DIM

srun -n1 --cpus-per-task=12 ./DOUBLE/gemm_openblas.exe $MATRIX_DIM $MATRIX_DIM $MATRIX_DIM

srun -n1 --cpus-per-task=12 ./DOUBLE/gemm_blis.exe $MATRIX_DIM $MATRIX_DIM $MATRIX_DIM


srun -n1 --cpus-per-task=12 ./FLOAT/gemm_mkl.exe $MATRIX_DIM $MATRIX_DIM $MATRIX_DIM

srun -n1 --cpus-per-task=12 ./FLOAT/gemm_openblas.exe $MATRIX_DIM $MATRIX_DIM $MATRIX_DIM

srun -n1 --cpus-per-task=12 ./FLOAT/gemm_blis.exe $MATRIX_DIM $MATRIX_DIM $MATRIX_DIM
