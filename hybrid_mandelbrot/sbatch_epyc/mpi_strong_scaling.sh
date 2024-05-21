#!/bin/bash
#SBATCH --job-name=m_mpi_strong
#SBATCH --output=mandelbrot_mpi_scaling_%j.out
#SBATCH --error=mandelbrot_mpi_scaling_%j.err
#SBATCH --nodes=2
#SBATCH --time=02:00:00
#SBATCH --partition=EPYC
#SBATCH --account=DSSC
#SBATCH --exclusive

# Load necessary modules
module load openMPI/4.1.5/gnu/12.2.1

# Compile
make -C ../src

# Set parameters
n_x=10000
n_y=10000
x_L=-2.0
y_L=-2.0
x_R=2.0
y_R=2.0
I_max=255

# Run with different numbers of MPI tasks
for ntasks in 1 2 4 8 16 32 64 96 128; do
    export OMP_NUM_THREADS=1
    mpiexec --map-by core -n $ntasks ../src/mandelbrot $n_x $n_y $x_L $y_L $x_R $y_R $I_max ../results/mpi_strong_scaling.csv
done
