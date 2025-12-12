#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
	bool (*query)(void *self, void *key, size_t len, double *delay, bool *fp);
	void (*insert)(void *self, void *key, size_t len);
} System;

#endif
