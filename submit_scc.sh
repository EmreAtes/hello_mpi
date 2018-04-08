#!/bin/bash -l
#$ -pe mpi_12_tasks_per_node 24

mpirun -n 24 hello.out &> scc_hello1.out &
mpirun -n 24 hello.out &> scc_hello2.out
