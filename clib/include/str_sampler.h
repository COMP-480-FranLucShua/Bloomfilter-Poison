#ifndef STR_SAMPLER_H
#define STR_SAMPLER_H

#include <stdint.h>
#include <stddef.h>

#include "interfaces/generator.h"
#include "interfaces/sampler.h"

extern const Sampler str_sampler_interface;

typedef struct Str_Sampler Str_Sampler;

/**
 * Create a new string sampler
 * 
 * Pass in an array of strings (note ownership is not passed to string sampler,
 * strings must outlive sampler)
 * 
 * The strings are sampled without replacement, until every string has been sampled, in which
 * case the sequence restarts (optionally with a new permutation)
 */
Str_Sampler* str_sampler_create(
	char **strings,
	size_t num_strings,
	Generator *rng,
	void *rng_inst
);

void str_sampler_destroy(Str_Sampler *self);

/**
 * Sample a single string
 * 
 * Returns a void * to the string sampled
 * 
 * Note the string returned is owned by the creator of the str_sampler
 */
void *str_sampler_sample(void *self, size_t *length);

/**
 * Sample an array of strings
 * 
 * samples and sample_lens must be preallocated
 */
void str_sampler_sample_array(void *self, void **samples, size_t *sample_lens, size_t array_size);

/**
 * Get total underlying array size
 */
size_t str_sampler_length(void *self);

#endif
