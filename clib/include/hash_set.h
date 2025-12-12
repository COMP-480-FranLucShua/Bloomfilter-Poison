#ifndef HASH_SET_H
#define HASH_SET_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "interfaces/set.h"

extern const Set hash_set_interface;

typedef struct HashSet HashSet;

typedef uint32_t (*HSetHashFunction)(const uint8_t *val, size_t len, uint32_t seed);

HashSet * hset_new(size_t starting_capacity, uint32_t seed, HSetHashFunction hash_function);
void * hset_destroy(void *);
HashSet *hset_clone(void *);

void hset_insert(void *, void *item, size_t size);
bool hset_query(void *, void *item, size_t size);
void hset_delete(void *, void *item, size_t size);

size_t hset_length(void *);

// *** ITERATOR
void *hset_next(void *, size_t *ptr);

// *** for debugging
void hset_print_int(void *);
void hset_print_str(void *);

void hset_dump_table(void *);

#endif // HASH_SET_H