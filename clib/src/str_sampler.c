#include "interfaces/sampler.h"

#include "str_sampler.h"
#include "rand_generator.h"

#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

void str_sampler_sample(void *self, char *sample, size_t *length);

Sampler str_sampler_interface = {
	.sample = str_sampler_sample
};

struct Str_Sampler {
	char **strings;
	uint32_t *keys;
	uint32_t idx;
};

Str_Sampler* str_sampler_create(char **strings, uint64_t seed) {
	uint32_t num_strings;
	uint32_t *keys;
	RandomGenerator *rng;

	Str_Sampler *smplr = malloc(sizeof(Str_Sampler));
	
	// Generate sample order
	num_strings = sizeof(*strings);
	keys = malloc(num_strings * sizeof(uint32_t));
	rng = rng_new(seed, seed);
	rng_generate_int32_bounded_array(rng, num_strings, keys, num_strings);
	rng_destroy(rng);

	smplr->keys = keys;
	smplr->strings = strings;
	smplr->idx = 0;

	return smplr;
}

void str_sampler_destroy(Str_Sampler *self) {
	free(self->keys);
	free(self);
}

void str_sampler_sample(void *self, char* sample, size_t *length) {
	Str_Sampler *str_smplr = self;

	sample = str_smplr->strings[str_smplr->keys[str_smplr->idx]];
	size_t len = strlen(sample);
	*length = len;

	str_smplr->idx++;
}
