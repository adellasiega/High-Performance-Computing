#!/bin/bash
#SBATCH --job-name=m_omp_weak
#SBATCH --output=mandelbrot_omp_weak_scaling_%j.out
#SBATCH --error=mandelbrot_omp_weak_scaling_%j.err
#SBATCH --nodes=2
#SBATCH --time=00:30:00
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


# Run with different numbers of OpenMP threads and proportionally increased problem size
for n_threads in {2..64..2}; do
    export OMP_NUM_THREADS=$n_threads
    export OMP_PLACES=cores
    scaled_n_threads=$(int_sqrt $n_threads)
    n_x=$(($n_x_base * $scaled_n_threads))
    n_y=$(($n_y_base * $scaled_n_threads))
    mpirun --map-by scocket --bind-to none -np 1  ../src/mandelbrot $n_x $n_y $x_L $y_L $x_R $y_R $I_max ../results/omp_weak_scaling.csv
done
