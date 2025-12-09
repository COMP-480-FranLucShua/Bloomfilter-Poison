#ifndef BIT_VECTOR_H
#define BIT_VECTOR_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
    size_t len;
    size_t _num_bytes;
    uint8_t _vector[]; // flexible array for bit data
} BitVector;

BitVector *bvec_new(size_t len);
void *bvec_destroy(BitVector * bit_vec);

/**
 * if bit_val is 0, evaluates to 0
 * otherwise, evaluates to 1
 */
BitVector *bvec_fill(BitVector *bit_vec, uint_fast8_t bit_val);
BitVector *bvec_set_bit(BitVector *bit_vec, size_t bit_index, uint_fast8_t bit_val);
uint_fast8_t bvec_get_bit(BitVector *bit_vec, size_t bit_index);

BitVector *bvec_clone(BitVector *bit_vec);

#endif