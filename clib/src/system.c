#include <stdlib.h>
#include "system.h"

/**
 * SystemEmulator does not own set, bfilter, or rng
 * Creator of SystemEmulator must free set, bfilter, and rng after use
 */
struct SystemEmulator {
    HashSet *set;
    BloomFilter *bfilter;
    RandomGenerator *rng;
    double system_delay;
};

SystemEmulator *sys_new(HashSet *set, BloomFilter *bfilter, RandomGenerator *rng, double delay) {
    SystemEmulator *sys = (SystemEmulator *)malloc(sizeof(SystemEmulator));

    sys->set = set;
    sys->bfilter = bfilter;
    sys->rng = rng;
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

    hset_insert(sys->set, data, len);
    bfilter_insert(sys->bfilter, data, len);
}

SystemEmulator *sys_insert_array(SystemEmulator *sys, void **data_array, size_t array_size, size_t data_len) {
    for (size_t i = 0; i < array_size; i++) {
        sys_insert(sys, data_array[i], data_len);
    }

    return sys;
}

bool sys_query(void *self, void *data, size_t len, double *delay, bool *fp) {
    SystemEmulator *sys = (SystemEmulator *)self;

    bool in_filter = bfilter_query(sys->bfilter, data, len);
    bool in_set = hset_query(sys->set, data, len);
    
    // include fixed delay for a positive bfilter check
    double query_delay = sys->system_delay * (double)in_filter;

    // include random delay according to rng distribution
    query_delay += rng_generate_gamma(sys->rng);
    
    *delay = query_delay;

    *fp = in_filter && !in_set;

    return in_set;
}

bool *sys_query_array(SystemEmulator *sys, void **data_array, size_t array_size, size_t data_len, bool *queries, double *delays, size_t *fp_count) {
    size_t false_pos_count = 0;
    for (size_t i = 0; i < array_size; i++) {
        bool query_fp;

        queries[i] = sys_query(sys, data_array[i], data_len, &delays[i], &query_fp);

        false_pos_count += query_fp;
    }
    *fp_count = false_pos_count;

    return queries;
}