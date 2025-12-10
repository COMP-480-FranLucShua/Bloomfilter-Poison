#ifndef SYSTEM_EMULATOR_H
#define SYSTEM_EMULATOR_H

#include <stdbool.h>
#include "hash_set.h"
#include "bloom_filter.h"
#include "rand_generator.h"

typedef struct {
    HashSet *set;
    BloomFilter *bfilter;
    RandomGenerator *rng;
    double system_delay;
} SystemEmulator;


/**
 * Ownership of set, bfilter, and rng are passed to this object
 */
SystemEmulator *sys_new(HashSet *set, BloomFilter *bfilter, RandomGenerator *rng, double delay);
void *sys_destroy(SystemEmulator *sys);
SystemEmulator *sys_clone(SystemEmulator *sys);

SystemEmulator *sys_insert(SystemEmulator *, void *data, size_t len);
SystemEmulator *sys_insert_array(SystemEmulator *, void **data_array, size_t array_size, size_t data_len);

/**
 * Stores delay result in delay variable
 */
bool sys_query(SystemEmulator *, void *data, size_t len, double *delay, bool *fp);

/**
 * Caller is in charge of passing an allocated query result array
 * 
 * if actual query results are not required, set corresponding results = NULL
 * if false positives are not required, set fps=NULL
 */
bool *sys_query_array(SystemEmulator *, void **data_array, size_t array_size, size_t data_len, bool *queries, double *delays, size_t *fp_count);

#endif