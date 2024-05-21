#include "timing_utils.h"
#include <stdio.h>

void save_timing_result(const char *results_filename, double start_time, double end_time,
                        int rank, int n_x, int n_y, double x_L, double y_L, double x_R, double y_R,
                        int I_max, int n_threads, int mpi_size) {
    if (rank == 0) {
        FILE *fp = fopen(results_filename, "a");
        if (fp == NULL) {
            fprintf(stderr, "Error opening timing file\n");
            return;
        }

        // Check if the file is empty to write the header
        fseek(fp, 0, SEEK_END);
        if (ftell(fp) == 0) {
            fprintf(fp, "mpi_size,omp_threads,n_x,n_y,x_L,y_L,x_R,y_R,I_max,time_seconds\n");
        }

        double elapsed_time = end_time - start_time;

        fprintf(fp, "%d,%d,%d,%d,%.2f,%.2f,%.2f,%.2f,%d,%.8f\n", 
                mpi_size, n_threads, n_x, n_y, x_L, y_L, x_R, y_R, I_max, elapsed_time);
        fclose(fp);
    }
}