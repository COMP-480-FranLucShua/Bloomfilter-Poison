#ifndef ATTACKER_H
#define ATTACKER_H

#include <stddef.h>

typedef struct {
	void (*attack)(void *self, size_t attack_size);
} Attacker;

#endif