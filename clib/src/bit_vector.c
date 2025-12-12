#include "bit_vector.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

BitVector *bvec_new(size_t len) {
    size_t num_bytes = len/8;
    if (len % 8 != 0) {
        num_bytes ++;
    }

    BitVector *bvec = (BitVector *)malloc(sizeof(BitVector) + num_bytes);

    bvec->len = len;
    bvec->_num_bytes = num_bytes;

    return bvec;
}

static inline uint8_t get_bit_val(uint_fast8_t bit_val) {
    return (uint8_t)!!bit_val;
}

void *bvec_destroy(BitVector *bvec) {
    if (bvec != NULL) {
        free(bvec);
        bvec = NULL;
    }
    return bvec;
}

BitVector *bvec_fill(BitVector *bvec, uint_fast8_t bit_val) {
    bit_val = get_bit_val(bit_val); // bit_val is 1 or 0

    // if bit_val is 0, remain 0. if bit_val is 1, result is 0xFF
    uint8_t fill_val = -bit_val;

    for (size_t i = 0; i < bvec->_num_bytes; i++) {
        bvec->_vector[i] = fill_val;
    }

    return bvec;
}

BitVector *bvec_set_bit(BitVector *bvec, size_t bit_index, uint_fast8_t bit_val) {
    if (bit_index >= bvec->len)
        return bvec;
    
    size_t byte_index = bit_index / 8;
    uint_fast8_t bit_in_byte = bit_index % 8;

    uint8_t bit_mask = 1 << bit_in_byte;

    uint8_t bit_val_byte = get_bit_val(bit_val);

    // clear bit
    bvec->_vector[byte_index] &= ~bit_mask;

    // set bit
    bvec->_vector[byte_index] |= (bit_val_byte << bit_in_byte);

    return bvec;
}

uint_fast8_t bvec_get_bit(BitVector *bvec, size_t bit_index) {
    if (bit_index >= bvec->len)
        return (uint_fast8_t) 0;
    
    size_t byte_index = bit_index / 8;
    uint_fast8_t bit_in_byte = bit_index % 8;

    uint8_t bit_mask = 1 << bit_in_byte;

    return (uint_fast8_t)get_bit_val(bvec->_vector[byte_index] & bit_mask);
}

BitVector *bvec_clone(BitVector *bvec) {
    size_t total_size = sizeof(BitVector) + bvec->_num_bytes;
    BitVector *new_bvec = (BitVector *)malloc(total_size);

    memcpy(new_bvec, bvec, total_size);

    return new_bvec;
}

void bvec_dump(BitVector *bvec) {
    fprintf(stderr, "[");
    for (size_t i = 0; i < bvec->len; i++) {
        fprintf(stderr, "%u", (uint32_t)bvec_get_bit(bvec, i));
    }
    fprintf(stderr, "]\n");
}