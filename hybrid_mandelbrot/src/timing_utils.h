#ifndef TIMING_UTILS_H
#define TIMING_UTILS_H

void save_timing_result(const char *results_filename, double start_time, double end_time,
                        int rank, int n_x, int n_y, double x_L, double y_L, double x_R, double y_R,
                        int I_max, int n_threads, int mpi_size);

#endif // TIMING_UTILS_H