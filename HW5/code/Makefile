CXX ?= g++
CC ?= gcc
CFLAGS = -Wall -Wconversion -O3 -fPIC
LIBS = blas/blas.a
#LIBS = -lblas
SHVER = 5
OS = $(shell uname)
ifeq ($(OS),Darwin)
	SHARED_LIB_FLAG = -dynamiclib -Wl,-install_name,liblinear.so.$(SHVER)
else
	SHARED_LIB_FLAG = -shared -Wl,-soname,liblinear.so.$(SHVER)
endif

all: train predict p10 p11 p12

lib: linear.o newton.o blas/blas.a
	$(CXX) $(SHARED_LIB_FLAG) linear.o newton.o blas/blas.a -o liblinear.so.$(SHVER)

train: newton.o linear.o train.c blas/blas.a
	$(CXX) $(CFLAGS) -o train train.c newton.o linear.o $(LIBS)

predict: newton.o linear.o predict.c blas/blas.a
	$(CXX) $(CFLAGS) -o predict predict.c newton.o linear.o $(LIBS)

p10: newton.o linear.o p10.cpp blas/blas.a
	$(CXX) $(CFLAGS) -o p10 p10.cpp newton.o linear.o $(LIBS)

p11: newton.o linear.o p11.cpp blas/blas.a
	$(CXX) $(CFLAGS) -o p11 p11.cpp newton.o linear.o $(LIBS)

p12: newton.o linear.o p12.cpp blas/blas.a
	$(CXX) $(CFLAGS) -o p12 p12.cpp newton.o linear.o $(LIBS)

# utils.o: utils.cpp utils.h 
# 	$(CXX) $(CFLAGS) -c -o utils.o utils.cpp

newton.o: newton.cpp newton.h
	$(CXX) $(CFLAGS) -c -o newton.o newton.cpp

linear.o: linear.cpp linear.h
	$(CXX) $(CFLAGS) -c -o linear.o linear.cpp

blas/blas.a: blas/*.c blas/*.h
	make -C blas OPTFLAGS='$(CFLAGS)' CC='$(CC)';

clean:
	make -C blas clean
	# make -C matlab clean
	rm -f *~ newton.o linear.o p10 p11 main train predict liblinear.so.$(SHVER)
