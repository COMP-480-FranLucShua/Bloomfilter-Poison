#include <stdio.h>
#include <string.h>
#include <time.h>
#include "t_bvec.h"
#include "bit_vector.h"
#include "rand_generator.h"

void *t_bvec_setup(const MunitParameter params[], void *user_data) {
    BitVector *bvec;

    bvec = bvec_new(64);
    bvec_fill(bvec, 0);

    return bvec;
}
void t_bvec_teardown(void *fixture) {
    BitVector *bvec = (BitVector *)fixture;
    bvec_destroy(bvec);
}

MunitResult t_bvec_fill(const MunitParameter params[], void *fixture) {
    BitVector *bvec = (BitVector *)fixture;
    bvec_dump(bvec);

    bvec_fill(bvec, 1);
    bvec_dump(bvec);

    for (size_t i = 0; i < bvec->len; i++) {
        uint8_t bit = (uint8_t)bvec_get_bit(bvec, i);
        munit_assert_uint8(bit, ==, 1);
    }

    bvec_fill(bvec, 0);
    bvec_dump(bvec);

    for (size_t i = 0; i < bvec->len; i++) {
        uint8_t bit = (uint8_t)bvec_get_bit(bvec, i);
        munit_assert_uint8(bit, ==, 0);
    }

    return MUNIT_OK;
}

MunitResult t_bvec_get_set_bit(const MunitParameter params[], void *fixture) {
    BitVector *bvec = (BitVector *)fixture;

    RandomGenerator *rng = rng_new(123456ULL, (uint64_t)time(NULL));

    uint32_t *bit_index_sequence = (uint32_t *)malloc(bvec->len * sizeof(uint32_t));

    rng_generate_choice(rng, bvec->len, bit_index_sequence, bvec->len);

    for (uint32_t i = 0; i < bvec->len; i++) {
        size_t bit_index = (size_t)bit_index_sequence[i];

        for (uint_fast8_t b = 0; b < 12; b++) {
            uint_fast8_t bit = b % 2;
            bvec_set_bit(bvec, bit_index, bit);
            uint8_t bit_read = (uint8_t)bvec_get_bit(bvec, bit_index);

            munit_assert_uint8(bit_read, ==, (uint8_t)bit);
        }
    }

    free(bit_index_sequence);

    return MUNIT_OK;
}

MunitTest t_bvec[] = {
    {
        "/bvec-fill",
        t_bvec_fill,
        t_bvec_setup,
        t_bvec_teardown,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/bvec-get-set-bit",
        t_bvec_get_set_bit,
        t_bvec_setup,
        t_bvec_teardown,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        NULL,
        NULL,
        NULL,
        NULL,
        0,
        NULL,
    },
};

const MunitSuite t_bvec_suite = {
    "/bvec-tests",
    t_bvec,
    NULL,
    1,
    MUNIT_SUITE_OPTION_NONE,
};