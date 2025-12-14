#ifndef timing_attacker_st_H
#define timing_attacker_st_H

#ifdef __cplusplus
extern "C" {
#endif

#include "interfaces/system.h"
#include "interfaces/sampler.h"
#include "interfaces/attacker.h"

// Opaque struct -- implementation is hidden in the .c file
typedef struct TimingAttackerST TimingAttackerST;

extern const Attacker timing_attacker_st_interface;

/**
 * Create a TimingAttackerST instance.
 *
 * @param fltr          Pointer to a Filter interface vtable
 * @param fltr_inst     Instance implementing the Filter interface
 * @param smplr         Pointer to a Sampler interface vtable
 * @param smplr_inst    Instance implementing the Sampler interface
 * @param training_proportion Proportion of attack size to use as training for KMeans Clustering
 *
 * @return Pointer to a newly allocated TimingAttackerST, or NULL on failure.
 */
TimingAttackerST* timing_attacker_st_create(
    const System *sstm, void *sstm_inst,
    const Sampler *smplr, void *smplr_inst
);

/**
 * Destroy a TimingAttackerST instance.
 */

void timing_attacker_st_destroy(TimingAttackerST *ta);

/**
 * Direct access attack method.
 * Users may call this directly if they know the concrete type.
 */
void timing_attacker_st_attack(void *ta, size_t attack_size);

#ifdef __cplusplus
}
#endif

#endif // timing_attacker_st_H

