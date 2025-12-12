#ifndef SYSTEM_EMULATOR_H
#define SYSTEM_EMULATOR_H

#include <stdbool.h>
#include "interfaces/system.h"
#include "interfaces/generator.h"
#include "interfaces/filter.h"
#include "interfaces/set.h"

typedef struct SystemEmulator SystemEmulator;

extern const System system_emulator_interface;

void sys_insert(void *, void *data, size_t len);
/**
 * Stores delay result in delay, and fp result in result variable variable
 */
bool sys_query(void *, void *data, size_t len, double *delay, bool *fp);


SystemEmulator *sys_new(Set *set, void *set_inst,
                        Filter *filter, void *filter_inst,
                        Generator *rng, void *rng_inst,
                        double delay);
void *sys_destroy(SystemEmulator *sys);

void sys_insert_array(void *, void **data_array, size_t *data_lens, size_t array_size);

/**
 * Caller is in charge of passing an allocated query result array
 * 
 * if actual query results are not required, set corresponding results = NULL
 * if false positives are not required, set fps=NULL
 */
void sys_query_array(void *, void **data_array, size_t *data_lens, size_t array_size, bool *queries, double *delays, size_t *fp_count);

#endif