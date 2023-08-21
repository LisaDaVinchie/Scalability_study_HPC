## MKL libraries
###
###
MKL= -L${MKLROOT}/lib/intel64  -lmkl_intel_lp64 -lmkl_gnu_thread -lmkl_core -lgomp -lpthread -lm -ldl

### OpenBLAS libraries 
OPENBLASROOT=${OPENBLAS_ROOT}
OPENBLAS = -L${OPENBLAS_ROOT}/lib -lopenblas -lpthread
### BLIS library
BLISROOT=/u/dssc/ldavin00/myblis

BLIS = -L$(BLISROOT)/lib -lpthread -lblis

FLOAT_FLAGS = gcc -DUSE_FLOAT
DOUBLE_FLAGS = gcc -DUSE_DOUBLE

GEMM=../gemm.c

DATA_FOLDER=${data_folder}

MKL_f=$(DATA_FOLDER)/gemm_mkl_f.exe
OPENBLAS_f=$(DATA_FOLDER)/gemm_openblas_f.exe
BLIS_f=$(DATA_FOLDER)/gemm_blis_f.exe

MKL_d=$(DATA_FOLDER)/gemm_mkl_d.exe
OPENBLAS_d=$(DATA_FOLDER)/gemm_openblas_d.exe
BLIS_d=$(DATA_FOLDER)/gemm_blis_d.exe



float: $(MKL_f) $(OPENBLAS_f) $(BLIS_f)

$(MKL_f) : $(GEMM)
	$(FLOAT_FLAGS) -DMKL $^ -m64 -I${MKLROOT}/include $(MKL)  -o $@ -DSAVE_RESULTS

$(OPENBLAS_f) : $(GEMM)
	$(FLOAT_FLAGS) -DOPENBLAS $^ -m64 -I${OPENBLAS_ROOT}/include $(OPENBLAS)  -o $@ -fopenmp -DSAVE_RESULTS

$(BLIS_f) : $(GEMM)
	$(FLOAT_FLAGS) -DBLIS $^ -m64 -I$(BLISROOT)/include/blis $(BLIS) -o $@ -fopenmp -DSAVE_RESULTS


double: $(MKL_d) $(OPENBLAS_d) $(BLIS_d)

$(MKL_d) : $(GEMM)
	$(DOUBLE_FLAGS) -DMKL $^ -m64 -I${MKLROOT}/include $(MKL)  -o $@ -DSAVE_RESULTS

$(OPENBLAS_d) : $(GEMM)
	$(DOUBLE_FLAGS) -DOPENBLAS $^ -m64 -I${OPENBLAS_ROOT}/include $(OPENBLAS)  -o $@ -fopenmp -DSAVE_RESULTS

$(BLIS_d) : $(GEMM)
	$(DOUBLE_FLAGS) -DBLIS $^ -m64 -I$(BLISROOT)/include/blis $(BLIS)  -o $@ -fopenmp -DSAVE_RESULTS	


# cpu: gemm_mkl.x gemm_oblas.x #gemm_blis.x


# gemm_mkl.x: $(GEMM)
# 	gcc -DUSE_DOUBLE -DMKL $^ -m64 -I${MKLROOT}/include $(MKL)  -o $@

# gemm_oblas.x: $(GEMM)
# 	gcc -DUSE_DOUBLE -DOPENBLAS $^ -m64 -I${OPENBLASROOT}/include -L/${OPENBLASROOT}/lib -lopenblas -lpthread -o $@ -fopenmp

#gemm_blis.x: $(GEMM)
#	gcc -DUSE_DOUBLE  -DBLIS $^ -m64 -I${BLISROOT}/include/blis -L/${BLISROOT}/lib -o $@ -lpthread  -lblis -fopenmp -lm
clean:
	rm -rf *.exe
