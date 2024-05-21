#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <omp.h>
#include "mandelbrot.h"
#include "image_utils.h"
#include "timing_utils.h"

int main(int argc, char *argv[]) {
	
	// Parse command line arguments
	if (argc != 9) {
		fprintf(stderr, "Usage: %s n_x n_y x_L y_L x_R y_R I_max results_filename\n", argv[0]);
		return 1;
	}

	int n_x = atoi(argv[1]);
	int n_y = atoi(argv[2]);
	double x_L = atof(argv[3]);
	double y_L = atof(argv[4]);
	double x_R = atof(argv[5]);
	double y_R = atof(argv[6]);
	int I_max = atoi(argv[7]);
	char *results_filename = argv[8];

	// Initialize MPI
	MPI_Init(&argc, &argv);
	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	MPI_Datatype MPI_PIXEL_TYPE;
	if (sizeof(PIXEL) == sizeof(unsigned char)) {
		MPI_Type_contiguous(sizeof(unsigned char), MPI_UNSIGNED_CHAR, &MPI_PIXEL_TYPE);
	} 
	else if (sizeof(PIXEL) == sizeof(unsigned short)) {
		MPI_Type_contiguous(sizeof(unsigned short), MPI_UNSIGNED_SHORT, &MPI_PIXEL_TYPE);
	}
	else {
		fprintf(stderr, "Unsupported PIXEL type\n");
		MPI_Abort(MPI_COMM_WORLD, 1);
	}
	MPI_Type_commit(&MPI_PIXEL_TYPE);

	// Start timer
	double start_time = MPI_Wtime();

	// Divide the image into quasi-equal parts
	const int complete_image_n_pixels = n_x * n_y;
	int partial_image_n_pixels = complete_image_n_pixels / size;
	int remainder_pixels = complete_image_n_pixels % size;
	
	int *recvcounts = (int *)malloc(size * sizeof(int));
	int *displs = (int *)malloc(size * sizeof(int));
	if (recvcounts == NULL || displs == NULL) {
		fprintf(stderr, "Error allocating memory for recvcounts or displs\n");
		MPI_Abort(MPI_COMM_WORLD, 1);
	}
	
	int sum = 0;
	for (int i = 0; i < size; i++) {
		recvcounts[i] = partial_image_n_pixels;
		if (i < remainder_pixels) {
			recvcounts[i] += 1;
		}
		displs[i] = sum;
		sum += recvcounts[i];
	}

	partial_image_n_pixels = recvcounts[rank];
	int start_pixel = displs[rank];
	int end_pixel = start_pixel + recvcounts[rank];

	// Allocate memory for partial images
	PIXEL *partial_image = (PIXEL *)malloc(partial_image_n_pixels * sizeof(PIXEL));
	if (partial_image == NULL) {
		fprintf(stderr, "Error allocating memory for partial image\n");
		MPI_Abort(MPI_COMM_WORLD, 1);
	}

	// Allocate memory for complete image only on root process
	PIXEL *complete_image = NULL;
	if (rank == 0) {
		complete_image = (PIXEL *)malloc(complete_image_n_pixels * sizeof(PIXEL));
		if (complete_image == NULL) {
			fprintf(stderr, "Error allocating memory for complete image\n");
			MPI_Abort(MPI_COMM_WORLD, 1);
		}
	}

	// Compute partial image
	compute_partial_image(n_x, n_y, x_L, y_L, x_R, y_R,
						  start_pixel, end_pixel, I_max,
						  partial_image, rank, size);

	// Gather partial images to complete image
	MPI_Gatherv(partial_image, partial_image_n_pixels, MPI_PIXEL_TYPE,
				complete_image, recvcounts, displs, MPI_PIXEL_TYPE,
				0, MPI_COMM_WORLD);

	// Stop timer
	double end_time = MPI_Wtime();
	
	// Save timing results
	if (rank == 0) {
		int n_threads;
		#pragma omp parallel 
		n_threads = omp_get_num_threads();
		save_timing_result(results_filename, start_time, end_time, rank, n_x, n_y, x_L, y_L, x_R, y_R, I_max, n_threads, size);
	}

	// Save image
	if (rank == 0) {
		char image_filename[100];
		sprintf(image_filename, "../images/m_%d_%d_%d_%d_%d_%d_%d.pgm", n_x, n_y, (int)x_L, (int)y_L, (int)x_R, (int)y_R, I_max);
		save_image(image_filename, complete_image, n_x, n_y);
	}

	free(recvcounts);
	free(displs);
	free(partial_image);
	free(complete_image);
	
	MPI_Type_free(&MPI_PIXEL_TYPE);
	MPI_Finalize();

	return 0;
}