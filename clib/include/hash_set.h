#ifndef HASH_SET_H
#define HASH_SET_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct HashSet HashSet;

typedef uint32_t (*HSetHashFunction)(const uint8_t *val, size_t len, uint32_t seed);

HashSet * hset_new(size_t starting_capacity, uint32_t seed, HSetHashFunction hash_function);
void * hset_destroy(HashSet *);
HashSet *hset_clone(HashSet *);

HashSet *hset_insert(HashSet *, void *item, size_t size);
HashSet *hset_insert_owned(HashSet *, void *item, size_t size);
bool hset_query(HashSet *, void *item, size_t size);
HashSet *hset_delete(HashSet *, void *item, size_t size);

// *** ITERATOR
void *hset_next(HashSet *, size_t *ptr);

// *** for debugging
void hset_print_int(HashSet *);
void hset_print_str(HashSet *);

void hset_dump_table(HashSet *);

#endif // HASH_SET_H