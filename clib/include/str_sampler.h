#ifndef STR_SAMPLER_H
#define STR_SAMPLER_H

#include <stdint.h>
typedef struct Str_Sampler Str_Sampler;

Str_Sampler* str_sampler_create(
	char **strings,
	uint64_t seed
);

void str_sampler_destroy(void *self);

char* sample(void *self);

#endif
