#ifndef TIMING_ATTACKER_H
#define TIMING_ATTACKER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "interfaces/filter.h"
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
 * @param time_threshold Threshold (in seconds) for timing attack classification
 *
 * @return Pointer to a newly allocated TimingAttacker, or NULL on failure.
 */
TimingAttacker* timing_attacker_create(
    const System *sstm, void *sstm_inst,
    const Sampler *smplr, void *smplr_inst,
    double time_threshold
);

/**
 * Destroy a TimingAttacker instance.
 */

void timing_attacker_destroy(TimingAttacker *ta);

/**
 * Direct access attack method.
 * Users may call this directly if they know the concrete type.
 */
void timing_attacker_attack(void *ta);

#ifdef __cplusplus
}
#endif

#endif // TIMING_ATTACKER_H

