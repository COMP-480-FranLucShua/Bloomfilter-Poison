#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "bloom_filter.h"
#include "interfaces/filter.h"
#include "interfaces/generator.h"
#include "bit_vector.h"

size_t bf_get_num_hf(void *self);
size_t bf_get_filter_len(void *self);
BitVector *bf_get_filter(void *self);

const Filter bloom_filter_interface = {
    .query = bfilter_query,
    .insert = bfilter_insert,
	.get_num_hf = bf_get_num_hf,
	.get_filter = bf_get_filter,
};

typedef uint32_t (*HashFunction)(const uint8_t *val, size_t len, uint32_t seed);

struct BloomFilter {
    BitVector *filter;
    HashFunction hash_function;
    size_t filter_range;
    size_t k;
    uint32_t seeds[]; // flexible array of seeds
};

BloomFilter *bfilter_new(size_t size, size_t k, Generator *rng_interface, void *rng_instance) {
    BloomFilter *bfilter = (BloomFilter *)malloc(sizeof(BloomFilter) + sizeof(uint32_t) * k);

    bfilter->filter_range = size;
    bfilter->k = k;
    bfilter->filter = bvec_new(bfilter->filter_range);

    rng_interface->gen_integers_unique(rng_instance, bfilter->seeds, k);

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

void bfilter_insert(void *self, void *data, size_t data_len) {
    BloomFilter *bfilter = (BloomFilter *)self;
    for (size_t i = 0; i < bfilter->k; i++) {
        size_t idx = bfilter->hash_function(data, data_len, bfilter->seeds[i]);
        bvec_set_bit(bfilter->filter, idx, 1);
    }
}

bool bfilter_query(void *self, void *data, size_t data_len) {
    BloomFilter *bfilter = (BloomFilter *)self;
    uint_fast8_t result = 0;
    for (size_t i = 0; i < bfilter->k; i++) {
        size_t idx = bfilter->hash_function(data, data_len, bfilter->seeds[i]);
        result |= bvec_get_bit(bfilter->filter, idx);
    }
    return (bool)result;
}

BloomFilter *bfilter_clone(BloomFilter * bfilter) {
    size_t total_size = sizeof(*bfilter) + sizeof(uint32_t) * bfilter->k;
    BloomFilter *bfilter_new = (BloomFilter *)malloc(total_size);
    memcpy(bfilter_new, bfilter, total_size);
    bfilter_new->filter = bvec_clone(bfilter->filter);

    return bfilter_new;
}

size_t bf_get_num_hf(void *self) {
    BloomFilter *bf = self;
    return bf->k;
}

size_t bf_get_filter_len(void *self) {
    BloomFilter *bf = self;
    return bf->filter_range;
}

BitVector *bf_get_filter(void *self) {
    BloomFilter *bf = self;
    return bf->filter;
}
