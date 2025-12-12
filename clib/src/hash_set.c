#include "hash_set.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

static inline uint32_t round_up_to_pow2(uint32_t x) {
    if (x == 0)
        return 1;
    
    x --;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x++;

    return x;
}

/**
 * SIDE EFFECT:
 * modifies perturb, should reset at start of each probing sequence
 */
static inline size_t probe(size_t i, uint32_t mask, uint32_t *perturb) {
    i = (i * 5) + 1 + *perturb;
    i &= mask;
    *perturb >>= 5;

    return i;
}

static inline size_t table_insert(HSet_Bucket *table, uint32_t mask, void *item, size_t size, uint32_t hash, bool *found);
static inline size_t table_lookup(HSet_Bucket *table, uint32_t mask, void *item, size_t size, uint32_t hash, bool *found);

HashSet *hset_resize(HashSet *);

HashSet *hset_new(size_t starting_capacity, uint32_t seed, HSetHashFunction hash_function) {
    HashSet *hset = (HashSet *)malloc(sizeof(HashSet));
    hset->capacity = (size_t)round_up_to_pow2((uint32_t)starting_capacity);
    hset->fill = 0;
    hset->used = 0;
    hset->mask = (uint32_t) hset->capacity-1;
    hset->seed = seed;
    hset->hash_function = hash_function;
    hset->table = (HSet_Bucket *)malloc(hset->capacity * sizeof(HSet_Bucket));

    return hset;
}
void *hset_destroy(HashSet *hset) {
    if (hset != NULL) {
        if (hset->table != NULL) {
            free(hset->table);
            hset->table = NULL;
        }
        free(hset);
        hset = NULL;
    }
    return hset;
}

HashSet *hset_clone(HashSet *hset) {
    HashSet *new_hash_set = (HashSet *)malloc(sizeof(HashSet));
    memcpy(new_hash_set, hset, offsetof(HashSet, table));

    new_hash_set->table = (HSet_Bucket *)malloc(hset->capacity * sizeof(HSet_Bucket));

    memcpy(new_hash_set->table, hset->table, hset->capacity * sizeof(HSet_Bucket));

    return new_hash_set;
}

HashSet *hset_insert(HashSet *hset, void *item, size_t size) {
    uint32_t hash = hset->hash_function((const uint8_t *)item, size, hset->seed);

    bool found = false;

    table_insert(hset->table, hset->mask, item, size, hash, &found);
    
    if (found) {
        // already contained
        return hset;
    }
    
    // successful insertion

    hset->fill ++;
    hset->used ++;

    // capacity check
    size_t threshold = (size_t)(0.67 * hset->capacity);
    if (hset->fill > threshold) {
        hset_resize(hset);
    }

    return hset;
}

bool hset_query(HashSet *hset, void *item, size_t size) {
    uint32_t hash = hset->hash_function((const uint8_t *)item, size, hset->seed);

    bool found = false;

    table_lookup(hset->table, hset->mask, item, size, hash, &found);

    return found;
}

HashSet *hset_delete(HashSet *hset, void *item, size_t size) {
    uint32_t hash = hset->hash_function((const uint8_t *)item, size, hset->seed);

    bool found = false;

    size_t idx = table_lookup(hset->table, hset->mask, item, size, hash, &found);

    if (found) {
        // found, time to delete
        HSet_Bucket *bucket = &hset->table[idx];

        bucket->item = NULL;
        bucket->size = 0;
        bucket->hash = 0;
        bucket->status = DUMMY;

        hset->used --;
    }

    return hset;
}

void *hset_next(HashSet *hset, size_t *ptr) {
    size_t idx = *ptr;
    for (; hset->table[idx].status != ACTIVE && idx < hset->capacity; idx++);

    *ptr = idx+1;

    if (idx >= hset->capacity) {
        return NULL;
    }
    

    return hset->table[idx].item;
}

void hset_print_int(HashSet *hset) {
    size_t idx = 0;
    void *item = hset_next(hset, &idx);
    
    printf("{ ");
    while (item != NULL) {
        uint32_t *item_ptr = (uint32_t *)item;
        uint32_t item_int = *item_ptr;
        printf("%u, ", item_int);

        item = hset_next(hset, &idx);
    }
    printf(" }\n");
}

void hset_print_str(HashSet *hset) {
    printf("{ ");
    size_t idx;
    void *item = hset_next(hset, &idx);

    while (item != NULL) {
        printf("%s, ", (char *)item);

        item = hset_next(hset, &idx);
    }
    printf(" }\n");
}

void hset_dump_table(HashSet *hset) {
    printf("[ ");
    for (size_t i = 0; i < hset->capacity; i++) {
        HSet_ProbeState status = hset->table[i].status;
        char status_code = (status == ACTIVE)? 'A' : (status == DUMMY)? 'D' : ' ';
        printf("%c, ", status_code);
    }
    printf(" ]\n");
}

static inline size_t table_insert(HSet_Bucket *table, uint32_t mask, void *item, size_t size, uint32_t hash, bool *found) {
    size_t idx = table_lookup(table, mask, item, size, hash, found);

    if (*found) {
        return 0;
    }

    // ready to insert

    HSet_Bucket *bucket = &table[idx];

    // now insert into bucket
    bucket->item = item;
    bucket->size = size;
    bucket->hash = hash;
    bucket->status = ACTIVE;

    return idx;
}

static inline size_t table_lookup(HSet_Bucket *table, uint32_t mask, void *item, size_t size, uint32_t hash, bool *found) {
    size_t idx = hash & mask;
    uint32_t perturb = hash;

    HSet_Bucket *bucket = &table[idx];

    while (bucket->status == ACTIVE) {
        // need to keep probing
        if (bucket->hash == hash && bucket->size == size) {
            // check equality
            if (memcmp(bucket->item, item, size) == 0) {
                // items are equal, already inserted
                *found = true;
                break;
            }
        }

        idx = probe(idx, mask, &perturb);
        bucket = &table[idx];
    }

    return idx;
}


HashSet *hset_resize(HashSet * hset) {
    size_t old_capacity = hset->capacity;
    size_t new_capacity = (size_t)round_up_to_pow2((uint32_t) old_capacity+1);
    size_t new_mask = new_capacity-1;
    HSet_Bucket *old_table = hset->table;
    HSet_Bucket *new_table = (HSet_Bucket *)malloc(new_capacity * sizeof(HSet_Bucket));

    // set hset to new values
    hset->capacity = new_capacity;
    hset->fill = 0;
    hset->used = 0;
    hset->mask = new_mask;
    hset->table = new_table;

    // reinsert all values
    HSet_Bucket *bucket;
    for (size_t i = 0; i < old_capacity; i++) {
        bucket = &old_table[i];
        if (bucket->status == ACTIVE) {
            bool found = false;
            // insert into new table
            table_insert(hset->table, hset->mask, bucket->item, bucket->size, bucket->hash, &found);
            hset->fill ++;
            hset->used ++;
        }
    }

    // delete old table
    free(old_table);
    return hset;
}