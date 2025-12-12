#ifndef RAND_GENERATOR_H
#define RAND_GENERATOR_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

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
void *rng_destroy(RandomGenerator *rng);

RandomGenerator *rng_clone(RandomGenerator *rng);

void rng_set_gamma_params(RandomGenerator *rng, double alpha, double theta);

// *** VALUE GENERATOR FUNCTIONS
/**
 * Generate a random unsigned 32 bit integer
 */
uint32_t rng_generate_int32(RandomGenerator *rng);
uint32_t rng_generate_int32_bounded(RandomGenerator *rng, size_t bound);

/**
 * Generate an array of random unsigned 32 bit integers
 * 
 * Assumes array is already allocated with the necessary size
 * 
 * returns array
 */
uint32_t * rng_generate_int32_array(RandomGenerator *rng, uint32_t *array, size_t len);
uint32_t * rng_generate_int32_bounded_array(RandomGenerator *rng, size_t bound, uint32_t *array, size_t len);

double rng_generate_double(RandomGenerator *rng);
double * rng_generate_double_array(RandomGenerator *rng, double *array, size_t len);

// *** RANDOM SELECTION

/**
 * Generate a random choice of integers in [0, range-1) without replacement
 *
 * This function should be used for relatively large values of len, and small values of range
 * 
 * Assumes array is already allocated with the necessary size
 */
uint32_t * rng_generate_choice(RandomGenerator *rng, size_t range, uint32_t *array, size_t len);

/**
 * Generate an array of uint32s, without replacement.
 *
 * This function is O(len^2), so should only be used for small values of len
 */
uint32_t * rng_generate_int32_array_unique(RandomGenerator *rng, uint32_t *array, size_t len);

// *** RANDOM DISTRIBUTIONS

double rng_generate_gamma(RandomGenerator *rng);
double rng_generate_gamma_params(RandomGenerator *rng, double alpha, double theta);

#endif // RAND_GENERATOR_H