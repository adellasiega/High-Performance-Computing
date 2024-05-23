#!/bin/bash
#SBATCH --job-name=m_omp_strong
#SBATCH --output=mandelbrot_omp_scaling_%j.out
#SBATCH --error=mandelbrot_omp_scaling_%j.err
#SBATCH --nodes=2
#SBATCH --time=00:30:00
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

# Run with different numbers of OpenMP threads
for n_threads in {2..64..2}; do
    export OMP_NUM_THREADS=$n_threads
    export OMP_PLACES=cores
    mpirun --map-by socket --bind-to none -np 1 ../src/mandelbrot $n_x $n_y $x_L $y_L $x_R $y_R $I_max ../results/omp_strong_scaling.csv
done
