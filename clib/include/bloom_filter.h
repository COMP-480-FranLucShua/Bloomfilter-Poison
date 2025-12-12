#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H
#include <stddef.h>
#include <stdbool.h>
#include "interfaces/generator.h"
#include "interfaces/filter.h"
#include "bit_vector.h"

extern const Filter bloom_filter_interface;

typedef struct BloomFilter BloomFilter;

typedef uint32_t (*BFilter_HashFunction)(const uint8_t *val, size_t len, uint32_t seed);

BloomFilter *bfilter_new(size_t size, size_t k, BFilter_HashFunction hash_function, Generator *rng_interface, void *rng_instance);
void *bfilter_destroy(BloomFilter *);

void bfilter_insert(void *, void *data, size_t data_len);
bool bfilter_query(void *, void *data, size_t data_len);

BloomFilter *bfilter_clone(BloomFilter *);

size_t bf_get_num_hf(void *self);
size_t bf_get_filter_len(void *self);
BitVector *bf_get_filter(void *self);

#endif // BLOOM_FILTER_H
