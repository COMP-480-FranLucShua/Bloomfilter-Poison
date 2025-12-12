#ifndef FILTER_H
#define FILTER_H

#include "bit_vector.h"

#include <stddef.h>
#include <stdint.h>

typedef struct {
	size_t (*get_num_hf)(void *self);
	size_t (*get_filter_len)(void *self);
	BitVector *(*get_filter)(void *self);
} Filter;

#endif
