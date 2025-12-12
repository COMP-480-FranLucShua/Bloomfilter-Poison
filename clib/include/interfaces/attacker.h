#ifndef ATTACKER_H
#define ATTACKER_H

typedef struct {
	void (*attack)(void *self);
} Attacker;

#endif
