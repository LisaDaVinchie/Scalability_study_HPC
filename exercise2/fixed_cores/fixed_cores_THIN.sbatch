#!/bin/bash

#Do not automatically requeue the job if there is a system failure
#SBATCH --no-requeue

#Name of the job that will be displayed in the queue
#SBATCH --job-name="size_scal"

#Partition that will be used
#SBATCH --partition=THIN

#Number of nodes that will be requested
#SBATCH -N 1

#Number of processes that will be executed in parallel
#SBATCH -n 12

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
sbatch_path=$(pwd)
cd
cd blis

blis_path=/u/dssc/ldavin00/myblis

srun -n1 ./configure --enable-cblas --enable-threading=openmp --prefix=${blis_path} auto
srun  -n 1 --cpus-per-task=12 make -j 12
make install
echo blis installed 
#Make the variable LD_LIBRARY_PATH an environmental variable, i.e. make it accessible by programs and scripts
export LD_LIBRARY_PATH=${blis_path}/lib:$LD_LIBRARY_PATH
echo returning to $sbatch_path
cd $sbatch_path

#Make the variable LD_LIBRARY_PATH an environmental variable, i.e. make it accessible by programs and scripts
export LD_LIBRARY_PATH=${blis_path}/lib:$LD_LIBRARY_PATH

echo setting number of cores and their topology
ncores=12
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

#Specify how many cores of the partition will be used
export OMP_NUM_THREADS=$ncores
echo settings exported
echo

echo establishing path to results
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
make double data_folder=${result_path}
make float data_folder=${result_path}
echo files compiled


#execute 
echo move to the result path
cd ${result_path}
echo moved to ${result_path}

echo run files
for size in $(seq 2000 500 20000)
do
	for iteration in $(seq 1 1 5)
	do
		#run the executables using 1 node and a fixed number of cores
		srun -n1 --cpus-per-task=$ncores gemm_mkl_d.exe $size $size $size
		srun -n1 --cpus-per-task=$ncores gemm_openblas_d.exe $size $size $size
		srun -n1 --cpus-per-task=$ncores gemm_blis_d.exe $size $size $size
		srun -n1 --cpus-per-task=$ncores gemm_mkl_f.exe $size $size $size
                srun -n1 --cpus-per-task=$ncores gemm_openblas_f.exe $size $size $size
                srun -n1 --cpus-per-task=$ncores gemm_blis_f.exe $size $size $size
	done
done

echo files run
echo delete exe files
make clean data_folder=${result_path}
echo exe files deleted
echo unload all the modules
module purge
echo modules unloaded
echo delete blis
rm -r ${blis_path}
echo blis deleted
echo program terminated
