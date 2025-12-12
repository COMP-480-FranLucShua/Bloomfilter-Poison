#ifndef SYSTEM_EMULATOR_H
#define SYSTEM_EMULATOR_H

#include <stdbool.h>
#include "hash_set.h"
#include "bloom_filter.h"

typedef struct {
    HashSet *set;
    BloomFilter *bfilter;
    RandomGenerator *rng;
    double fixed_delay;
} SystemEmulator;

typedef struct {
    double delay;
    bool query;
} SystemQuery;

/**
 * Ownership of set, bfilter, and rng are passed to this object
 */
SystemEmulator *sys_new(HashSet *set, BloomFilter *bfilter, RandomGenerator *rng, double delay);
void *sys_destroy(SystemEmulator *sys);
SystemEmulator *sys_clone(SystemEmulator *sys);

// SystemEmulator *sys_insert(void *data, size_t len);
// SystemEmulator *sys_insert_array(void **data_array, size_t array_size, size_t data_len);

// SystemQuery sys_query(void *data, size_t len);

/**
 * Caller is in charge of passing an allocated query result array
 * 
 * if actual query results are not required, set results = NULL
 * if false positives are not required, set fps=NULL
 */
// SystemQuery *sys_query_array(void **data_array, size_t array_size, size_t data_len, SystemQuery * results, size_t *fps);

// size_t sys_get_fps(void **data_array, size_t array_size, size_t data_len);

#endif