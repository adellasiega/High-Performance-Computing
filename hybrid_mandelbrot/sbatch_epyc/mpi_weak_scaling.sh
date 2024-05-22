#!/bin/bash
#SBATCH --job-name=m_mpi_weak
#SBATCH --output=mandelbrot_mpi_weak_scaling_%j.out
#SBATCH --error=mandelbrot_mpi_weak_scaling_%j.err
#SBATCH --nodes=2
#SBATCH --time=02:00:00
#SBATCH --partition=EPYC
#SBATCH --account=DSSC
#SBATCH --exclusive

# Load necessary modules
module load openMPI/4.1.5/gnu/12.2.1

# Compile
make -C ../src

# Base problem size
n_x_base=2000
n_y_base=2000

# Set other parameters
x_L=-2.0
y_L=-2.0
x_R=2.0
y_R=2.0
I_max=255

# Function to calculate the integer square root
int_sqrt() {
    local num=$1
    local sqrt_result

    # Calculate the square root using bc and round it to the nearest integerusing awk
    sqrt_result=$(echo "scale=10; sqrt($num)" | bc | awk '{printf "%.0f\n", $1}')
 
    echo $sqrt_result
}

# Run with different numbers of MPI tasks and proportionally increased problem size
for ntasks in 1 2 4 8 16 32 64 80 96 112 128; do
    export OMP_NUM_THREADS=1
    scaled_n_tasks=$(int_sqrt $ntasks)
    n_x=$(($n_x_base * $scaled_n_tasks))
    n_y=$(($n_y_base * $scaled_n_tasks))
    mpiexec --map-by core -n $ntasks ../src/mandelbrot $n_x $n_y $x_L $y_L $x_R $y_R $I_max ../results/mpi_weak_scaling.csv
done
