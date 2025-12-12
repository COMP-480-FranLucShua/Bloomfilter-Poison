#ifndef SAMPLER_H
#define SAMPLER_H

#include <stddef.h>

typedef struct {
	void *(*sample)(void *self, size_t *length);
	void (*sample_array)(void *self, void **samples, size_t *sample_lens, size_t array_size);
	size_t (*length)(void *self);
} Sampler;

#endif