#!/bin/bash

#Do not automatically requeue the job if there is a system failure
#SBATCH --no-requeue

#Name of the job that will be displayed in the queue
#SBATCH --job-name="core_scal"

#Partition that will be used
#SBATCH --partition=THIN

#Number of nodes that will be requested
#SBATCH -N 1

#Number of processes that will be executed in parallel
#SBATCH -n 24

#The job has exclusive acces to the requested node
#SBATCH --exclusive

#Maximum time
#SBATCH --time=2:00:00

#Name of the output of this sbatch file
#SBATCH --output="summary.out"

echo Memory allocated
echo
echo Loading modules
module load architecture/Intel
module load mkl
module load openBLAS/0.3.23-omp
echo modules loaded

#Make the variable LD_LIBRARY_PATH an environmental variable, i.e. make it accessible by programs and scripts
export LD_LIBRARY_PATH=/u/dssc/ldavin00/myblis/lib:$LD_LIBRARY_PATH

echo setting number of cores and their topology
size=10000
node=THIN
#alloc=spread
alloc=close
echo setted number of cores and their topology
echo
echo exporting settings

# setting number of cores and their topology
##Place each OpenMP thread on an individual cpu core
export OMP_PLACES=cores

##Specyfy how threads and processing units are distribuited: 
##close: the distance between the thread and the processing unit is minimised
##sparse: balance locality and load balance
export OMP_PROC_BIND=$alloc

echo settings exported
echo

#echo establishing path to results
#if [ "$node" = "THIN" ];
#then
#	result_path=./THIN
#elif [ "$node" = "EPYC" ];
#then
#	result_path=./EPYC
#fi
#
result_path=./THIN

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


echo compile files, specifying in which folder the results will be stored
make double data_folder=$result_path
make float data_folder=$result_path
echo files compiled


#execute 
echo move to the result path
cd ${result_path}
echo moved to ${result_path}

echo run files
for idx in $(seq 1 1 24)
do
	#Specify how many cores of the partition will be used
	export OMP_NUM_THREADS=$idx
	for iteration in $(seq 1 1 5)
	do
		#run the executables using 1 node and a fixed number of cores
		srun -n 1 --cpus-per-task=$idx gemm_mkl_d.exe $size $size $size
		srun -n 1 --cpus-per-task=$idx gemm_openblas_d.exe $size $size $size
		srun -n 1 --cpus-per-task=$idx gemm_blis_d.exe $size $size $size
		srun -n 1 --cpus-per-task=$idx gemm_mkl_f.exe $size $size $size
                srun -n 1 --cpus-per-task=$idx gemm_openblas_f.exe $size $size $size
                srun -n 1 --cpus-per-task=$idx gemm_blis_f.exe $size $size $size
	done
done

echo files run
#delete executable files
make clean
#unload all the modules
module purge

echo program terminated
