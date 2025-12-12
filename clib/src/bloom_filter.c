#include <string.h>
#include <stdlib.h>
#include "bloom_filter.h"
#include "rand_generator.h"

typedef uint32_t (*HashFunction)(const uint8_t *val, size_t len, uint32_t seed);

struct BloomFilter {
    BitVector *filter;
    HashFunction hash_function;
    size_t filter_range;
    size_t k;
    uint32_t seeds[]; // flexible array of seeds
};

BloomFilter *bfilter_new(size_t size, size_t k, RandomGenerator *rng) {
    BloomFilter *bfilter = (BloomFilter *)malloc(sizeof(BloomFilter) + sizeof(unit32_t) * k);

    bfilter->filter_range = size;
    bfilter->k = k;
    bfilter->filter = bvec_new(bfilter->filter_range);

    rng_generate_int32_array_unique(rng, bfilter->seeds, k);

    return bfilter;
}

void *bfilter_destroy(BloomFilter *bfilter) {
    if (bfilter != NULL) {
        bvec_destroy(bfilter->filter);
        free(bfilter);
        bfilter = NULL;
    }

    return bfilter;
}

BloomFilter *bfilter_insert(BloomFilter * bfilter, uint8_t *data, size_t data_len) {
    for (size_t i = 0; i < bfilter->k; i++) {
        size_t idx = bfilter->hash_function(data, data_len, bfilter->seeds[i]);
        bvec_set_bit(bfilter->filter, idx, 1);
    }

    return bfilter;
}

uint_fast8_t bfilter_query(BloomFilter * bfilter, uint8_t *data, size_t data_len) {
    uint_fast8_t result = 0;
    for (size_t i = 0; i < bfilter->k; i++) {
        size_t idx = bfilter->hash_function(data, data_len, bfilter->seeds[i]);
        result |= bvec_get_bit(bfilter->filter, idx);
    }
    return result;
}

BloomFilter *bfilter_clone(BloomFilter * bfilter) {
    size_t total_size = sizeof(*bfilter) + sizeof(unit32_t) * k;
    BloomFilter *bfilter_new = (BloomFilter *)malloc(total_size);
    memcpy(bfilter_new, bfilter, total_size);
    bfilter_new->filter = bvec_clone(bfilter->filter);

    return bfilter_new;
}