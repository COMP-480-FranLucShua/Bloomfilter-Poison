#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdbool.h>
#include <stddef.h>

#include "bit_vector.h"

typedef struct {
	bool (*query)(void *self, void *key, size_t len, double *delay, bool *fp);
	void (*insert)(void *self, void *key, size_t len);

	void (*insert_array)(void *self, void **keys, size_t *lens, size_t array_size);

	/**
	 * keys, lens, queries, and delays must be array_size len
	 * queries, delays and fp_count can be NULL to indicate
	 * return value is not needed
	 */
	void (*query_array)(void *self, void **keys, size_t *lens, size_t array_size, bool *queries, double *delays, size_t *fp_count);
} System;

#endif
