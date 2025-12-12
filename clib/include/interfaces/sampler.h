#ifndef SAMPLER_H
#define SAMPLER_H

#include <stddef.h>

typedef struct {
	void (*sample)(void *self, char *sample, size_t *length);
} Sampler;

#endif
