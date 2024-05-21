#ifndef MANDELBROT_H
#define MANDELBROT_H

#include <complex.h>
#include <omp.h>

#define PIXEL unsigned char

PIXEL mandelbrot(const double complex c, const int I_max);
void compute_partial_image(const int n_x, const int n_y,
                           const double x_L, const double y_L,
                           const double x_R, const double y_R,
                           const int start_pixel, const int end_pixel,
                           const int I_max, PIXEL *partial_image,
                           const int rank, const int size);

#endif // MANDELBROT_H