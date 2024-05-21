#ifndef IMAGE_UTILS_H
#define IMAGE_UTILS_H

#include "mandelbrot.h"

void save_image(const char *filename, const PIXEL *complete_image, const int n_x, const int n_y);

#endif // IMAGE_UTILS_H