#ifndef TIMING_ATTACKER_H
#define TIMING_ATTACKER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "interfaces/system.h"
#include "interfaces/sampler.h"

// Opaque struct -- implementation is hidden in the .c file
typedef struct TimingAttacker TimingAttacker;

/**
 * Create a TimingAttacker instance.
 *
 * @param fltr          Pointer to a Filter interface vtable
 * @param fltr_inst     Instance implementing the Filter interface
 * @param smplr         Pointer to a Sampler interface vtable
 * @param smplr_inst    Instance implementing the Sampler interface
 * @param training_proportion Proportion of attack size to use as training for KMeans Clustering
 *
 * @return Pointer to a newly allocated TimingAttacker, or NULL on failure.
 */
TimingAttacker* timing_attacker_create(
    const System *sstm, void *sstm_inst,
    const Sampler *smplr, void *smplr_inst,
    double training_proportion
);

/**
 * Destroy a TimingAttacker instance.
 */

void timing_attacker_destroy(TimingAttacker *ta);

/**
 * Direct access attack method.
 * Users may call this directly if they know the concrete type.
 */
void timing_attacker_attack(void *ta, size_t attack_size);

#ifdef __cplusplus
}
#endif

#endif // TIMING_ATTACKER_H

