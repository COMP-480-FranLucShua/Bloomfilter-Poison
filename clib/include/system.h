#ifndef SYSTEM_EMULATOR_H
#define SYSTEM_EMULATOR_H

#include <stdbool.h>
#include "interfaces/system.h"
#include "rand_generator.h"

typedef struct SystemEmulator SystemEmulator;

void sys_insert(void *, void *data, size_t len);
/**
 * Stores delay result in delay, and fp result in result variable variable
 */
bool sys_query(void *, void *data, size_t len, double *delay, bool *fp);

const System system_emulator_interface = {
    .query = sys_query,
    .insert = sys_insert,
};



/**
 * Ownership of set, bfilter, and rng are passed to this object
 */
SystemEmulator *sys_new(HashSet *set, BloomFilter *bfilter, RandomGenerator *rng, double delay);
void *sys_destroy(SystemEmulator *sys);
SystemEmulator *sys_clone(SystemEmulator *sys);

SystemEmulator *sys_insert_array(SystemEmulator *, void **data_array, size_t array_size, size_t data_len);



/**
 * Caller is in charge of passing an allocated query result array
 * 
 * if actual query results are not required, set corresponding results = NULL
 * if false positives are not required, set fps=NULL
 */
bool *sys_query_array(SystemEmulator *, void **data_array, size_t array_size, size_t data_len, bool *queries, double *delays, size_t *fp_count);

#endif