#ifndef NAIVE_ATTACKER_H
#define NAIVE_ATTACKER_H

#include "interfaces/system.h"
#include "interfaces/attacker.h"
#include "interfaces/sampler.h"

extern const Attacker naive_attacker_interface;

// Opaque struct -- implementation is hidden in the .c file
typedef struct Naive_Attacker Naive_Attacker;

Naive_Attacker* naive_attacker_create(
    const System *sstm, void *sstm_inst,
    const Sampler *smplr, void *smplr_inst
);

void naive_attacker_destroy(Naive_Attacker *na);

/**
 * perform a naive attack (random sampling attack data)
 */
void naive_attacker_attack(void *ta, size_t attack_size);


#endif // TIMING_ATTACKER_H
