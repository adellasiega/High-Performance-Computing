# High Performance Computing 

This repository contains code and resources related to High-Performance Computing projects. The project used the computational resource of ORFEO cluster.

## Contents
1. `hybrid_mandelbrot`: OpenMP+MPI Mandelbrot set implementation and analysis
    - `src`: C source code with Makefile for compiling
    - `images`: graphs and rendering of the set
    - `presentation`: tex files for the slides
    - `report`: tex files for the report
    - `results`: csv files containing strong and weak scaling timing measures
    - `sbatch_epyc`: SLURM job files for EPYC partition


2. `openMPI_collective_operations`: analysis of broadcast and reduce operations
    - `bcast`: csv files containing bcast timing measures
    - `plots`: plots of the data analysis on both bcast and reduce
    - `presentation`: tex files for the slides
    - `reduce`: csv files containing reduce timing measures
    - `report`: tex files for the report
    - `sbatch_thin`: SLURM job files for THIN partition
    - `.ipynb`: jupyter notebooks for EDA and linear model

3. `pdf` files of reports and presentations


## Author
Alessandro Della Siega

May 2024, University of Trieste