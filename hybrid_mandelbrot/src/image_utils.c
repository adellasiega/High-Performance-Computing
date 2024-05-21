#include "image_utils.h"
#include <stdio.h>

void save_image(const char *filename, const PIXEL *complete_image, const int n_x, const int n_y) {
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file\n");
        return;
    }
    fprintf(fp, "P5\n%d %d\n255\n", n_x, n_y);
    fwrite(complete_image, sizeof(PIXEL), n_x * n_y, fp);
    fclose(fp);
}