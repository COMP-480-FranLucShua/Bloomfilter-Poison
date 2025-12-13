#ifndef GENERATOR_H
#define GENERATOR_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint32_t (*gen_integer)(void *self); // generate a 
    uint32_t *(*gen_integers)(void *self, uint32_t *array, size_t count);
    uint32_t *(*gen_integers_unique)(void *self, uint32_t *array, size_t count);
    double (*gen_double)(void *self); // generate a value from a distribution
    double (*gen_distribution)(void *self); // generate a value from a distribution
    uint32_t *(*gen_choice)(void *self, uint32_t *array, size_t range, size_t count); // generate a random choice (without replacement)
} Generator;

#endif