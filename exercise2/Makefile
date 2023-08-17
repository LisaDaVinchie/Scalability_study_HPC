### MKL libraries
###
###
MKL= -L${MKLROOT}/lib/intel64  -lmkl_intel_lp64 -lmkl_gnu_thread -lmkl_core -lgomp -lpthread -lm -ldl

### OpenBLAS libraries 
OPENBLASROOT=${OPENBLAS_ROOT}
OPENBLAS = -L${OPENBLAS_ROOT}/lib
### BLIS library
#BLISROOT=/u/area/cozzini/myblis

float: FLOAT/mkl/gemm_mkl.exe FLOAT/openblas/gemm_openblas.exe FLOAT/blis/gemm_blis.exe

gemm_mkl.exe : gemm.c
	gcc -DUSE_FLOAT -DMKL $^ -m64 -I${MKLROOT}/include $(MKL)  -o $@

gemm_mkl.exe : gemm.c
	gcc -DUSE_FLOAT -DMKL $^ -m64 -I${OPENBLAS_ROOT}/include $(OPENBLAS)  -o $@

gemm_mkl.exe : gemm.c
	gcc -DUSE_FLOAT -DMKL $^ -m64 -I${MKLROOT}/include $(MKL)  -o $@	

# cpu: gemm_mkl.x gemm_oblas.x #gemm_blis.x


# gemm_mkl.x: gemm.c
# 	gcc -DUSE_DOUBLE -DMKL $^ -m64 -I${MKLROOT}/include $(MKL)  -o $@

# gemm_oblas.x: gemm.c
# 	gcc -DUSE_DOUBLE -DOPENBLAS $^ -m64 -I${OPENBLASROOT}/include -L/${OPENBLASROOT}/lib -lopenblas -lpthread -o $@ -fopenmp

#gemm_blis.x: gemm.c
#	gcc -DUSE_DOUBLE  -DBLIS $^ -m64 -I${BLISROOT}/include/blis -L/${BLISROOT}/lib -o $@ -lpthread  -lblis -fopenmp -lm
clean:
	rm -rf *.x
