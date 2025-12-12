#include <stdlib.h>
#include "system.h"
#include "interfaces/system.h"
#include "interfaces/generator.h"
#include "interfaces/filter.h"
#include "interfaces/set.h"

const System system_emulator_interface = {
    .query = sys_query,
    .insert = sys_insert,
    .query_array = sys_query_array,
    .insert_array = sys_insert_array,
};

/**
 * SystemEmulator does not own set, bfilter, or rng
 * Creator of SystemEmulator must free set, bfilter, and rng after use
 */
struct SystemEmulator {
    Set *set;
    void *set_inst;
    Filter *filter;
    void *filter_inst;
    Generator *rng;
    void *rng_inst;
    double system_delay;
};

SystemEmulator *sys_new(Set *set, void *set_inst,
                        Filter *filter, void *filter_inst,
                        Generator *rng, void *rng_inst,
                        double delay) {
    SystemEmulator *sys = (SystemEmulator *)malloc(sizeof(SystemEmulator));

    sys->set = set;
    sys->set_inst = set_inst;
    sys->filter = filter;
    sys->filter_inst = filter_inst;
    sys->rng = rng;
    sys->rng_inst = rng_inst;
    sys->system_delay = delay;

    return sys;
}

void *sys_destroy(SystemEmulator *sys) {
    if (sys != NULL) {
        free(sys);
        sys = NULL;
    }
    return sys;
}

void sys_insert(void *self, void *data, size_t len) {
    SystemEmulator *sys = (SystemEmulator *)self;

    sys->set->insert(sys->set_inst, data, len);
    sys->filter->insert(sys->filter_inst, data, len);
}

void sys_insert_array(void *self, void **data_array, size_t *data_lens, size_t array_size) {
    SystemEmulator *sys = self;

    for (size_t i = 0; i < array_size; i++) {
        sys_insert(sys, data_array[i], data_lens[i]);
    }
}

bool sys_query(void *self, void *data, size_t len, double *delay, bool *fp) {
    SystemEmulator *sys = (SystemEmulator *)self;

    bool in_filter = sys->filter->query(sys->filter_inst, data, len);
    bool in_set = sys->set->query(sys->set_inst, data, len);
    
    // include fixed delay for a positive bfilter check
    double query_delay = sys->system_delay * (double)in_filter;

    // include random delay according to rng distribution
    query_delay += sys->rng->gen_distribution(sys->rng_inst);
    
    *delay = query_delay;

    *fp = in_filter && !in_set;

    return in_set;
}

void sys_query_array(void *self, void **data_array, size_t *data_lens, size_t array_size, bool *queries, double *delays, size_t *fp_count) {
    SystemEmulator *sys = self;

    size_t false_pos_count = 0;
    for (size_t i = 0; i < array_size; i++) {
        bool query_fp;
        double delay;
        bool query;

        query = sys_query(sys, data_array[i], data_lens[i], &delay, &query_fp);

        if (queries)
            queries[i] = query;
        if (delays)
            delays[i] = delay;

        false_pos_count += query_fp;
    }

    if (fp_count)
        *fp_count = false_pos_count;
}