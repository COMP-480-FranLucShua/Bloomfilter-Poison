#ifndef HASH_FAMILY_H
#define HASH_FAMILY_H

#include <stdint.h>
#include "rand_generator.h"

#define HASH_FAMILY_MAX_SEEDS 16

typedef uint32_t (*HFamHashFunction)(const uint8_t *val, size_t len, uint32_t seed);

typedef struct
{
    size_t range;
    size_t k;                 // Number of hash functions
    HFamHashFunction hash_function; // hash function
    uint32_t seeds[HASH_FAMILY_MAX_SEEDS];           // flexible array of seeds (size = k)
} HashFamily;

// *** CONSTRUCTOR/DESTRUCTOR
void hfam_init(HashFamily *hash_fam, size_t range, size_t k, HFamHashFunction hash_function, RandomGenerator *rng);

/**
 * data is a datapoint_size byte data array
 */
uint32_t hfam_call(HashFamily *hash_fam, size_t hash_idx, uint8_t *data, size_t data_len);

#endif