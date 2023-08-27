#!/bin/bash

#Do not automatically requeue the job if there is a system failure
#SBATCH --no-requeue

#Name of the job that will be displayed in the queue
#SBATCH --job-name="core_scal"

#Partition that will be used
#SBATCH --partition=EPYC

#Number of nodes that will be requested
#SBATCH -N 1

#Number of processes that will be executed in parallel
#SBATCH -n 128

#The job has exclusive acces to the requested node
#SBATCH --exclusive

#Maximum time
#SBATCH --time=2:00:00

#Name of the output of this sbatch file
#SBATCH --output="summary.out"

echo Memory allocated
echo
echo Loading modules
module load architecture/AMD
module load mkl
module load openBLAS/0.3.23-omp
echo modules loaded

sbatch_path=$(pwd)
cd
cd blis

blis_path=/u/dssc/ldavin00/myblis

srun -n1 ./configure --enable-cblas --enable-threading=openmp --prefix=${blis_path} auto
srun  -n 1 --cpus-per-task=128 make -j 128
make install
echo blis installed
#Make the variable LD_LIBRARY_PATH an environmental variable, i.e. make it accessible by programs and scripts
export LD_LIBRARY_PATH=${blis_path}/lib:$LD_LIBRARY_PATH
echo returning to $sbatch_path
cd $sbatch_path
echo returned to previous path

echo setting number of cores and their topology
size=5000
node=EPYC
alloc=spread
#alloc=close
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

result_path=./EPYC

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
for idx in $(seq 1 1 128)
do
	echo Specify how many cores of the partition will be used
	export OMP_NUM_THREADS=$idx
	echo number of cores specified
	echo start 5 iterations
	for iteration in $(seq 1 1 5)
	do	
		#run the executables using 1 node and a fixed number of cores
		echo -n "$idx," >> mkl_d.csv
		#srun -n 1 --cpus-per-task=$idx gemm_mkl_d.exe $size $size $size
		./gemm_mkl_d.exe $size $size $size
		
		echo -n "$idx," >> openblas_d.csv
		#srun -n 1 --cpus-per-task=$idx gemm_openblas_d.exe $size $size $size
		./gemm_openblas_d.exe $size $size $size
		
		echo -n "$idx," >> blis_d.csv
		#srun -n 1 --cpus-per-task=$idx gemm_blis_d.exe $size $size $size
		./gemm_blis_d.exe $size $size $size
		
		echo -n "$idx," >> mkl_f.csv
		#srun -n 1 --cpus-per-task=$idx gemm_mkl_f.exe $size $size $size
                ./gemm_mkl_f.exe $size $size $size
		
		echo -n "$idx," >> openblas_f.csv
		#srun -n 1 --cpus-per-task=$idx gemm_openblas_f.exe $size $size $size
                ./gemm_openblas_f.exe $size $size $size
		
		echo -n "$idx," >> blis_f.csv
		#srun -n 1 --cpus-per-task=$idx gemm_blis_f.exe $size $size $size
		./gemm_blis_f.exe $size $size $size	
		
		echo Number of cores = $idx iteration=$iteration
	done
	echo iterations ended
done

echo files run
#delete executable files
make clean data_folder=$result_path
#unload all the modules
module purge

echo delete blis
rm -r ${blis_path}
echo program terminated
