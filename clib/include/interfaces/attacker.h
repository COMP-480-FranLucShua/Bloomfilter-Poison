#ifndef ATTACKER_H
#define ATTACKER_H

#include <stddef.h>

/**
 * Interface for an attacker
 * 
 * An attacker's attack function takes a parameter attack_size
 * 
 * An attacker may insert at most attack_size values into its system,
 * with the goal of maximizing the false positive rate
 */
typedef struct {
	void (*attack)(void *self, size_t attack_size);
} Attacker;

#endif