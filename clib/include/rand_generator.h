#ifndef RAND_GENERATOR_H
#define RAND_GENERATOR_H

#include "pcg/pcg_basic.h"
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    pcg32_random_t pcg;
    double gamma_alpha;
    double gamma_theta;
} RandomGenerator;

// *** CONSTRUCTOR/DESTRUCTOR

/**
 * Create a new random generator with a specified seed and sequence id
 * 
 * Parameters:
 * init_state - unsigned 64 bit starting seed
 * init_seq - unsigned 64 bit sequence ID
 */
RandomGenerator *rng_new(uint64_t init_state, uint64_t init_seq);

/**
 * Destroy a random number generator if not already destroyed
 * 
 * Returns null pointer on success
 * 
 * To prevent duplicate frees, should set rng variable to return result of function
 */
void *rng_destroy(RandomGenerator *rng);

RandomGenerator *rng_clone(RandomGenerator *rng);

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
 * Assumes array is already allocated with the necessary size
 */
uint32_t * rng_generate_choice(RandomGenerator *rng, size_t range, uint32_t *array, size_t len);


// *** RANDOM DISTRIBUTIONS

double rng_generate_gamma(RandomGenerator *rng);
double rng_generate_gamma_params(RandomGenerator *rng, double alpha, double theta);

#endif // RAND_GENERATOR_H