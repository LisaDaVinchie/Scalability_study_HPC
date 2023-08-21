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
# module load architecture/Intel
module load mkl
module load openBLAS/0.3.23-omp
echo modules loaded

export LD_LIBRARY_PATH=/u/dssc/ldavin00/myblis/lib:$LD_LIBRARY_PATH

echo setting number of cores and their topology
ncores=12
node=THIN
alloc=spread
#alloc=close
echo setted number of cores and their topology
echo
echo exporting settings
### setting number of cores and their topology
export OMP_PLACES=cores
export OMP_PROC_BIND=$alloc
export OMP_NUM_THREADS=$ncores
echo settings exported
echo

echo establishing path to results
if [ "$node" = "THIN" ];
then
	result_path=./THIN
elif [ "$node" = "EPYC" ];
then
	result_path=./EPYC
fi

if [ "$alloc" = "close" ];
then
	result_path="$result_path/close_cores"
elif [ "$alloc" = "spread" ];
then
	result_path="$result_path/spread_cores"
fi

## cd $(result_path)

echo path to results is ${result_path}

echo

echo compile files
make double data_folder=${result_path}
make float data_folder=${result_path}
echo files compiled

CPUS_PER_TASK=12

#execute requiring 12 cpus per task
echo move to the result path
cd ${result_path}
echo moved to ${result_path}

echo run files
for size in $(seq 2000 500 14000)
do
	for iteration in $(seq 1 1 5)
	do
		srun -n1 --cpus-per-task=$CPUS_PER_TASK gemm_mkl_d.exe $size $size $size
		srun -n1 --cpus-per-task=$CPUS_PER_TASK gemm_openblas_d.exe $size $size $size
		srun -n1 --cpus-per-task=$CPUS_PER_TASK gemm_blis_d.exe $size $size $size
		srun -n1 --cpus-per-task=$CPUS_PER_TASK gemm_mkl_f.exe $size $size $size
                srun -n1 --cpus-per-task=$CPUS_PER_TASK gemm_openblas_f.exe $size $size $size
                srun -n1 --cpus-per-task=$CPUS_PER_TASK gemm_blis_f.exe $size $size $size
	done
done

echo files run

make clean
module purge

echo program terminated
