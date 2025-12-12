#include "interface/sampler.h"
#include "str_sampler.h"
#include "rand_generator.h"

#include <cstdint>
#include <stdlib.h>
#include <stddef.h>

char* str_sampler_sample(void *self);

Sampler str_sampler_interface = {
	.sample = str_sampler_sample
};

typedef struct {
	char **strings;
	size_t *keys;
	size_t idx;
} Str_Sampler;

Str_Sampler* str_sampler_create(char **strings, uint64_t seed) {
	size_t num_strings;
	size_t *keys;
	RandomGenerator *rng;

	Str_Sampler *smplr = malloc(sizeof(Str_Sampler));
	
	// Generate sample order
	num_strings = sizeof(*strings);
	keys = malloc(num_strings);
	rng = rng_new(seed, seed);
	rng_generate_int32_bounded_array(rng, num_strings, keys, num_strings);
	rng_destroy(rng);

	smplr->keys = keys;
	smplr->strings = strings;
	smplr->idx = 0;
}

void str_sampler_destroy(Str_Sampler *self) {
	free(self);
}

char* str_sampler_sample(void *self) {
	char* sample;

	Str_Sampler *str_smplr = self;

	sample = str_smplr->strings[str_smplr->keys[str_smplr->idx]];
	str_smplr->idx++;

	return sample;
}
