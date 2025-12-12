#ifndef STR_SAMPLER_H
#define STR_SAMPLER_H

#include <stdint.h>
#include <stddef.h>

typedef struct Str_Sampler Str_Sampler;

Str_Sampler* str_sampler_create(
	char **strings,
	uint64_t seed
);

void str_sampler_destroy(Str_Sampler *self);


void str_sampler_sample(void *self, char *sample, size_t *length);

#endif
