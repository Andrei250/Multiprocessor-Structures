build: serial.cpp cocktail-openmp.cpp cocktail-pthreads.cpp
	g++ serial.cpp -o serial -fopenmp
	g++ cocktail-openmp.cpp -o cocktail-openmp -fopenmp
	g++ cocktail-pthreads.cpp -o cocktail-pthreads -lpthread
	mpic++ cocktail-mpi.cpp -o cocktail-mpi
	mpic++ cocktail-mpi-openmp.cpp -o cocktail-mpi-openmp -fopenmp

run-serial: serial.cpp
	./serial

run-openmp: cocktail-openmp.cpp
	./cocktail-openmp

run-pthread: cocktail-pthreads.cpp
	./cocktail-pthreads

run-mpi: cocktail-mpi.cpp 
	mpirun -np 4 ./cocktail-mpi

run-hybrid: cocktail-mpi-openmp.cpp build
	mpirun -np 4 ./cocktail-mpi-openmp

clean:
	rm -rf cocktail-openmp serial cocktail-pthreads cocktail-mpi