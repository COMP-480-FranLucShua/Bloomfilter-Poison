#ifndef KMEANS_1D_H
#define KMEANS_1D_H

#include <stddef.h>

/**
 * Compute a threshold that divides the input array into two clusters
 * using kmeans clustering
 * 
 * The result should be a threshold value *t* such that,
 * for any *x* > *t*, it belongs to the group in the input array > *t*,
 * and for any *x* < *t*, it belongs to the group in the input array < *t*
 */
double kmeans_threshold_1d(double *array, size_t array_size);

#endif