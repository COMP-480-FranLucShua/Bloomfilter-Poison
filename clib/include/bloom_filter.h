#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H


#include "bit_vector.h"
#include "hash_family.h"

typedef struct
{
    size_t filter_range;
    size_t k;
    HashFamily *hash_family;
    BitVector *filter;

} BloomFilter;

BloomFilter *bfilter_new(HashFamily *hash_family);
void *bfilter_destroy(BloomFilter *);

BloomFilter *bfilter_insert(BloomFilter *, uint8_t *data, size_t data_len);
uint_fast8_t bfilter_query(BloomFilter *, uint8_t *data, size_t data_len);

BloomFilter *bfilter_clone(BloomFilter *);

#endif // BLOOM_FILTER_H