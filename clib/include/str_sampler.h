#ifndef STR_SAMPLER_H
#define STR_SAMPLER_H

#include <stdint.h>
#include <stddef.h>

#include "interfaces/generator.h"
#include "interfaces/sampler.h"

extern const Sampler str_sampler_interface;

typedef struct Str_Sampler Str_Sampler;

Str_Sampler* str_sampler_create(
	char **strings,
	size_t num_strings,
	Generator *rng,
	void *rng_inst
);

void str_sampler_destroy(Str_Sampler *self);


void *str_sampler_sample(void *self, size_t *length);
void str_sampler_sample_array(void *self, void **samples, size_t *sample_lens, size_t array_size);
size_t str_sampler_length(void *self);

#endif
