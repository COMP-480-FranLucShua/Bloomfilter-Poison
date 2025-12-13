#include "kmeans1d.h"
#include "rand_generator.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <stdbool.h>

#define MAX_ITER 100

static void attach_points(double *points, size_t *attached, size_t num_points, double centroids[2]) {
	for (size_t point_idx = 0; point_idx < num_points; point_idx++) {
		double point = points[point_idx];

		size_t closest_centroid;
		double closest_dist = DBL_MAX;

		for (size_t centroid_idx = 0; centroid_idx < 2; centroid_idx++) {
			double centroid_location = centroids[centroid_idx];
			double dist = fabs(point-centroid_location);

			if (dist < closest_dist) {
				closest_dist = dist;
				closest_centroid = centroid_idx;
			}
		}

		attached[point_idx] = closest_centroid;
	}
}

static bool recalibrate_centroids(double *points, size_t *attached, size_t num_points, double centroids[2]) {
	bool changed = false;

	double centroid_point_sum[2] = {0.0, 0.0};
	uint32_t centroid_point_cnt[2] = {0, 0};

	for (size_t point_idx = 0; point_idx < num_points; point_idx++) {
		double point = points[point_idx];
		size_t attached_to = attached[point_idx];

		centroid_point_sum[attached_to] = centroid_point_sum[attached_to] + point;
		centroid_point_cnt[attached_to]++;
	}

	for (size_t centroid_idx = 0; centroid_idx < 2; centroid_idx++) {
		double old_centroid = centroids[centroid_idx];
		double new_centroid = centroid_point_sum[centroid_idx] / centroid_point_cnt[centroid_idx];

		if (fabs(old_centroid - new_centroid) > 1e-9) {
    			changed = true;
		}

		centroids[centroid_idx] = new_centroid;
	}

	return changed;
}

double kmeans_threshold_1d(double *array, size_t array_size) {
	size_t *attached = malloc(array_size * sizeof(size_t));
	double centroids[2];

	// Define Initial clusters
	RandomGenerator *rng = rng_new(0x12345678);

	uint32_t *sample_indices = (uint32_t *)malloc(array_size * sizeof(uint32_t));
	rng_generate_choice(rng, sample_indices, array_size, array_size);

	centroids[0] = array[sample_indices[0]];
	for (size_t i = 1; i < array_size; i++) {
		centroids[1] = array[sample_indices[i]];
		if (centroids[1] != centroids[0])
			break;
	}
	free(sample_indices);

	bool changed = true;
	size_t iter = 0;
	while (changed && iter < MAX_ITER) {
		attach_points(array, attached, array_size, centroids);
		changed = recalibrate_centroids(array, attached, array_size, centroids);
		iter++;
	}

	double threshold = (centroids[0] + centroids[1]) / 2;

	free(attached);
	rng_destroy(rng);
	return threshold;
}
