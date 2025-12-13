#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

#include "interfaces/sampler.h"
#include "interfaces/generator.h"

#include "str_sampler.h"


void *str_sampler_sample(void *self, size_t *length);

const Sampler str_sampler_interface = {
	.sample = str_sampler_sample,
	.sample_array = str_sampler_sample_array,
	.length = str_sampler_length
};

struct Str_Sampler {
	char **strings;
	size_t num_strings;
	uint32_t *keys;
	uint32_t idx;
	const Generator *rng;
	void *rng_inst;
};

Str_Sampler* str_sampler_create(char **strings, size_t num_strings, const Generator *rng, void *rng_inst) {
	uint32_t *keys;

	Str_Sampler *smplr = malloc(sizeof(Str_Sampler));
	
	// Generate sample order
	keys = (uint32_t *)malloc(num_strings * sizeof(uint32_t));
	rng->gen_choice(rng_inst, keys, num_strings, num_strings);

	smplr->strings = strings;
	smplr->num_strings = num_strings;
	smplr->keys = keys;
	smplr->idx = 0;
	smplr->rng = rng;
	smplr->rng_inst = rng_inst;

	return smplr;
}

void str_sampler_destroy(Str_Sampler *self) {
	free(self->keys);
	free(self);
}

void *str_sampler_sample(void *self, size_t *length) {
	Str_Sampler *str_smplr = (Str_Sampler *)self;

	if (str_smplr->idx >= str_smplr->num_strings) {
		// regenerate
		str_smplr->rng->gen_choice(str_smplr->rng_inst, str_smplr->keys, str_smplr->num_strings, str_smplr->num_strings);
		str_smplr->idx = 0;
	}

	size_t str_idx = str_smplr->keys[str_smplr->idx];

	void *sample = str_smplr->strings[str_idx];

	size_t len = strlen(sample);

	if (length)
		*length = len;

	str_smplr->idx++;

	return sample;
}

void str_sampler_sample_array(void *self, void **samples, size_t *sample_lens, size_t array_size) {
	for (size_t i = 0; i < array_size; i++) {
		samples[i] = str_sampler_sample(self, &sample_lens[i]);
	}
}

size_t str_sampler_length(void *self) {
	Str_Sampler *str_smplr = self;
	return str_smplr->num_strings;
}