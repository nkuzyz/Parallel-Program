EXECS=mpi_gause
MPICC?=mpicc
all: ${EXECS}
mpi_gause: mpi_gause.cpp
	${MPICC} -o mpi_gause mpi_gause.cpp -lstdc++
clean:
	rm -f ${EXECS}

