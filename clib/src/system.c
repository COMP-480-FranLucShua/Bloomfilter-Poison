#include "system.h"
#include <stdlib.h>

SystemEmulator *sys_new(HashSet *set, BloomFilter *bfilter, RandomGenerator *rng, double delay) {
    SystemEmulator *sys = (SystemEmulator *)malloc(sizeof(SystemEmulator));

    sys->set = set;
    sys->bfilter = bfilter;
    sys->rng = rng;
    sys->fixed_delay = delay;

    return sys;
}

void *sys_destroy(SystemEmulator *sys) {
    if (sys != NULL) {
        sys->set = hset_destroy(sys->set);
        sys->bfilter = bfilter_destroy(sys->bfilter);
        sys->rng = rng_destroy(sys->rng);

        free(sys);
        sys = NULL;
    }
    return sys;
}

SystemEmulator *sys_clone(SystemEmulator *sys) {
    SystemEmulator *new_sys = (SystemEmulator *) malloc(sizeof(SystemEmulator));
    new_sys->set = hset_clone(sys->set);
    new_sys->bfilter = bfilter_clone(sys->bfilter);
    new_sys->rng = rng_clone(sys->rng);
    new_sys->fixed_delay = sys->fixed_delay;

    return new_sys;
}

// SystemEmulator *sys_insert(void *data, size_t len) {
    
// }