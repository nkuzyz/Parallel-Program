EXECS=mpi_trap
MPICC?=mpicc
all: ${EXECS}
mpi_trap: mpi_trap.cpp
	${MPICC} -o mpi_trap mpi_trap.cpp
clean:
	rm -f ${EXECS}

