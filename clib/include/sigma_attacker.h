#ifndef SIGMA_ATTACKER_H
#define SIGMA_ATTACKER_H

#include "interfaces/filter.h"
#include "interfaces/attacker.h"
#include "interfaces/system.h"

extern const Attacker sigma_attacker_interface;

// Opaque struct -- implementation is hidden in the .c file
typedef struct Sigma_Attacker Sigma_Attacker;

Sigma_Attacker* sigma_attacker_create(
	const System *sys, void *sys_inst,
	const Filter *fltr, void *fltr_inst
);

void sigma_attacker_destroy(Sigma_Attacker *sa);

void sigma_attacker_attack(void *sa, size_t attack_size);


#endif
