#include "bloom_filter.h"
#include <string.h>
#include <stdlib.h>

BloomFilter *bfilter_new(HashFamily *hash_family) {
    BloomFilter *bfilter = (BloomFilter *)malloc(sizeof(BloomFilter));

    bfilter->filter_range = hash_family->range;
    bfilter->k = hash_family->k;
    bfilter->hash_family = hash_family;
    bfilter->filter = bvec_new(bfilter->filter_range);

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
        size_t idx = hfam_call(bfilter->hash_family, i, data, data_len);
        bvec_set_bit(bfilter->filter, idx, 1);
    }

    return bfilter;
}

uint_fast8_t bfilter_query(BloomFilter * bfilter, uint8_t *data, size_t data_len) {
    uint_fast8_t result = 0;
    for (size_t i = 0; i < bfilter->k; i++) {
        size_t idx = hfam_call(bfilter->hash_family, i, data, data_len);
        result |= bvec_get_bit(bfilter->filter, idx);
    }
    return result;
}

BloomFilter *bfilter_clone(BloomFilter * bfilter) {
    BloomFilter *bfilter_new = (BloomFilter *)malloc(sizeof(*bfilter));
    memcpy(bfilter_new, bfilter, offsetof(BloomFilter, filter));
    bfilter_new->filter = bvec_clone(bfilter->filter);

    return bfilter_new;
}