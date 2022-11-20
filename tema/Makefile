build: cocktail-openmp.cpp cocktail-pthreads.cpp
	g++ cocktail-openmp.cpp -o cocktail-openmp -fopenmp
	g++ cocktail-pthreads.cpp -o cocktail-pthreads -lpthread
	mpic++ cocktail-mpi.cpp -o cocktail-mpi
	mpic++ cocktail-mpi-openmp.cpp -o cocktail-mpi-openmp -fopenmp

run-openmp: cocktail-openmp.cpp
	./cocktail-openmp

run-pthread: cocktail-pthreads.cpp
	./cocktail-pthreads

run-mpi: cocktail-mpi.cpp 
	mpirun -np 4 ./cocktail-mpi

run-hybrid: cocktail-mpi-openmp.cpp
	mpirun -np 4 ./cocktail-mpi-openmp

run-all: run-openmp run-pthread run-mpi run-hybrid

clean:
	rm -rf cocktail-openmp cocktail-pthreads cocktail-mpi cocktail-mpi-openmp