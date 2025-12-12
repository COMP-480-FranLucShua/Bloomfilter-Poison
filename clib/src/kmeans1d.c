#include "kmeans1d.h"

double kmeans_threshold_1d(double *array, size_t array_size) {
    double val = array[0];

    return 0.5 * array_size + val;
}