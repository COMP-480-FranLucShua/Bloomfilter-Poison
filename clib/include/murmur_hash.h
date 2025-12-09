#ifndef MURMUR_HASH_H
#define MURMUR_HASH_H

#include <stdint.h>
#include <string.h>

/**
 * NOTE THAT CODE BELOW COMES FROM 
 * 
 * https://en.wikipedia.org/wiki/MurmurHash#cite_note-15
 * 
 * CITE PROPERLY
 */

uint32_t murmur3_32(const uint8_t *key, size_t len, uint32_t seed);

#endif