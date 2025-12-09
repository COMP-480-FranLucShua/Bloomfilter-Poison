#include <stdlib.h>
#include "hash_family.h"
#include "rand_generator.h"

void hfam_init(HashFamily *hash_fam, size_t range, size_t k, HFamHashFunction hash_function, RandomGenerator *rng) {
    hash_fam->range = range;
    hash_fam->k = k;
    hash_fam->hash_function = hash_function;
    
    rng_generate_int32_array(rng, hash_fam->seeds, k);
}

uint32_t hfam_call(HashFamily *hash_fam, size_t hash_idx, uint8_t *data, size_t data_len) {
    if (hash_idx >= hash_fam->k)
        return 0;
    return hash_fam->hash_function(data, data_len, hash_fam->seeds[hash_idx]);
}