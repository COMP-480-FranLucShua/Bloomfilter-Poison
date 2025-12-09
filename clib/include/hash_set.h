#ifndef HASH_SET_H
#define HASH_SET_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef uint32_t (*HSetHashFunction)(const uint8_t *val, size_t len, uint32_t seed);

typedef enum {
    EMPTY,
    ACTIVE,
    DUMMY
} HSet_ProbeState;

typedef struct {
    void *item; // the actual object hashed
    size_t size;
    uint32_t hash; // cached hash
    HSet_ProbeState status; // bucket status
} HSet_Bucket;

typedef struct {
    size_t capacity; // total number of buckets
    size_t fill; // # active/dummy slots
    size_t used; // # active slots
    uint32_t mask; // table size - 1
    uint32_t seed; // table hash seed
    HSetHashFunction hash_function; // hash function
    HSet_Bucket *table; // array of entries
} HashSet;

HashSet * hset_new(size_t starting_capacity, uint32_t seed, HSetHashFunction hash_function);
void * hset_destroy(HashSet *);
HashSet *hset_clone(HashSet *);

HashSet *hset_insert(HashSet *, void *item, size_t size);
bool hset_lookup(HashSet *, void *item, size_t size);
HashSet *hset_delete(HashSet *, void *item, size_t size);

// *** ITERATOR
void *hset_next(HashSet *, size_t *ptr);

// *** for debugging
void hset_print_int(HashSet *);
void hset_print_str(HashSet *);

void hset_dump_table(HashSet *);

#endif // HASH_SET_H