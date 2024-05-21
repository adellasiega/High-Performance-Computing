#include "mandelbrot.h"

PIXEL mandelbrot(const double complex c, const int I_max) {
    double complex z = 0;
    PIXEL iter = 0;
    while (cabs(z) < 2 && iter < I_max) {
        z = z * z + c;
        iter++;
    }
    if (iter == I_max) {
        iter = 0;
    }
    return iter;
}

void compute_partial_image(const int n_x, const int n_y,
                           const double x_L, const double y_L,
                           const double x_R, const double y_R,
                           const int start_pixel, const int end_pixel,
                           const int I_max, PIXEL *partial_image,
                           const int rank, const int size) {

    const double dx = (x_R - x_L) / n_x;
    const double dy = (y_R - y_L) / n_y;

    #pragma omp parallel for schedule(dynamic)
    for (int i = start_pixel; i < end_pixel; i++) {
        double x = x_L + (i % n_x) * dx;
        double y = y_L + (i / n_x) * dy;
        double complex c = x + I * y;
        partial_image[i-start_pixel] = mandelbrot(c, I_max);
    }
}