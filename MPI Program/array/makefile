EXECS=mpi_array
MPICC?=mpicc
all: ${EXECS}
mpi_array: mpi_array.cpp
	${MPICC} -o mpi_array mpi_array.cpp -lstdc++
clean:
	rm -f ${EXECS}

