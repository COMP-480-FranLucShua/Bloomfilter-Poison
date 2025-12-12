#ifndef RAND_GENERATOR_H
#define RAND_GENERATOR_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "interfaces/generator.h"

typedef struct RandomGenerator RandomGenerator;

// *** CONSTRUCTOR/DESTRUCTOR

/**
 * Create a new random generator with a specified seed and sequence id
 * 
 * Parameters:
 * seed - starting seed
 */
RandomGenerator *rng_new(uint32_t seed);

/**
 * Destroy a random number generator if not already destroyed
 * 
 * Returns null pointer on success
 * 
 * To prevent duplicate frees, should set rng variable to return result of function
 */
void *rng_destroy(void *self);

RandomGenerator *rng_clone(void *self);

void rng_set_gamma_params(void *self, double alpha, double theta);

// *** VALUE GENERATOR FUNCTIONS
/**
 * Generate a random unsigned 32 bit integer
 */
uint32_t rng_generate_int32(void *self);
uint32_t rng_generate_int32_bounded(void *self, size_t bound);

/**
 * Generate an array of random unsigned 32 bit integers
 * 
 * Assumes array is already allocated with the necessary size
 * 
 * returns array
 */
uint32_t * rng_generate_int32_array(void *self, uint32_t *array, size_t len);
uint32_t * rng_generate_int32_bounded_array(void *self, size_t bound, uint32_t *array, size_t len);

double rng_generate_double(void *self);
double * rng_generate_double_array(void *self, double *array, size_t len);

// *** RANDOM SELECTION

/**
 * Generate a random choice of integers in [0, range-1) without replacement
 *
 * This function should be used for relatively large values of len, and small values of range
 * 
 * Assumes array is already allocated with the necessary size
 */
uint32_t * rng_generate_choice(void *self, uint32_t *array, size_t range, size_t len);

/**
 * Generate an array of uint32s, without replacement.
 *
 * This function is O(len^2), so should only be used for small values of len
 */
uint32_t * rng_generate_int32_array_unique(void *self, uint32_t *array, size_t len);

// *** RANDOM DISTRIBUTIONS

double rng_generate_gamma(void *self);
double rng_generate_gamma_params(void *self, double alpha, double theta);


// *** IMPLEMENT INTERFACE
extern const Generator rand_generator_interface;

#endif // RAND_GENERATOR_H