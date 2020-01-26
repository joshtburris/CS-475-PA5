CFLAGS=-O3 --ptxas-options -v --gpu-architecture=sm_61 --compiler-bindir /usr/local/gcc-6.4.0/bin -std=c++11 -I/s/bach/c/under/joshtb/cuda-patches/include
LIBRARIES=-O3 -std=c99 -fopenmp -I/usr/include/malloc/ -lm
CC=gcc 
AR=xiar
OBJS = MMScan.o

all: sequential cuda

debug: CFLAGS =-DDEBUG -g -Wall -Wextra -std=c99 -I/usr/include/malloc/
debug: all

sequential:
	$(CC) MMScan.c -o MMScan.o $(LIBRARIES) -c
	$(CC) MMScan-wrapper.c -o MMScan $(OBJS) $(LIBRARIES) 

cuda:
	nvcc MMScanCUDA.cu -c -o MMScanCUDA.o $(CFLAGS) -DCUDA
	nvcc MMScan-wrapper.cu -o MMScanCUDA MMScanCUDA.o $(CFLAGS) -DCUDA -DG=625 -DS=32
	./MMScanCUDA 100000 64

test:
	for g in 400 500 625 800 1000 1250 ; do \
		for s in 8 16 32 ; do \
			nvcc MMScanCUDA.cu -c -o MMScanCUDA.o $(CFLAGS) -DCUDA ; \
			nvcc MMScan-wrapper.cu -o MMScanCUDA MMScanCUDA.o $(CFLAGS) -DCUDA -DG=$$g -DS=$$s ; \
			./MMScanCUDA 100000 64  >> results.txt; \
		done ; \
		echo "" >> results.txt ; \
	done

## Add additional line for different versions like verb, rand, etc. (for debugging/testing)

## Then add similar sets of lines for other executables


clean:
	rm -f *.o MMScan MMScan.verb MMScan.int MMScan.rand MMScan.verb-rand MMScan.check MMScanKernel01 MMScanKernel01.int 
