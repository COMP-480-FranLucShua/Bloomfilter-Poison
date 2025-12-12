#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H
#include <stddef.h>
#include "rand_generator.h"

typedef struct BloomFilter BloomFilter;

BloomFilter *bfilter_new(size_t size, size_t k, RandomGenerator *rng);
BloomFilter *bfilter_new(HashFamily *hash_family);
void *bfilter_destroy(BloomFilter *);

BloomFilter *bfilter_insert(BloomFilter *, uint8_t *data, size_t data_len);
uint_fast8_t bfilter_query(BloomFilter *, uint8_t *data, size_t data_len);

BloomFilter *bfilter_clone(BloomFilter *);

#endif // BLOOM_FILTER_H