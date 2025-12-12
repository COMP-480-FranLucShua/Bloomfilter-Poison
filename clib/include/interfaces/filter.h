#ifndef FILTER_H
#define FILTER_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "bit_vector.h"

typedef struct {
	void (*insert)(void *self, void *data, size_t len);
	bool (*query)(void *self, void *data, size_t len);
	size_t (*get_num_hf)(void *self);
	BitVector *(*get_filter)(void *self);
} Filter;

#endif
