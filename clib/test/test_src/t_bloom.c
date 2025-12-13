#include <stdlib.h>
#include <string.h>
#include "t_bloom.h"
#include "bloom_filter.h"
#include "interfaces/filter.h"
#include "rand_generator.h"
#include "murmur_hash.h"

/* -------------------------------------------------- */
/* Setup / Teardown                                   */
/* -------------------------------------------------- */

void *t_bloom_setup(const MunitParameter params[], void *user_data) {
    void *rng_inst = rng_new(12345);

    BloomFilter *bf = bfilter_new(
        128,    /* bit vector size */
        3,      /* number of hash functions */
        murmur3_32,
        &rand_generator_interface,
        rng_inst
    );

    return bf;
}

void t_bloom_teardown(void *fixture) {
    BloomFilter *bf = fixture;

    bfilter_destroy(bf);
}

/* -------------------------------------------------- */

MunitResult t_bloom_empty_query(const MunitParameter params[], void *fixture) {
    BloomFilter *bf = fixture;

    int x = 42;
    munit_assert_false(bfilter_query(bf, &x, sizeof(x)));

    return MUNIT_OK;
}

/* -------------------------------------------------- */

MunitResult t_bloom_insert_query(const MunitParameter params[], void *fixture) {
    BloomFilter *bf = fixture;

    int x = 10;
    int y = 20;

    bfilter_dump(bf);

    bfilter_insert(bf, &x, sizeof(x));

    bfilter_dump(bf);

    munit_assert_true(bfilter_query(bf, &x, sizeof(x)));

    /* false positives allowed, but not required */
    (void)y;

    return MUNIT_OK;
}

/* -------------------------------------------------- */

MunitResult t_bloom_no_false_negatives(const MunitParameter params[], void *fixture) {
    BloomFilter *bf = fixture;

    const int count = 50;
    int values[count];

    for (int i = 0; i < count; i++) {
        values[i] = i * 3 + 1;
        bfilter_insert(bf, &values[i], sizeof(int));
    }

    for (int i = 0; i < count; i++) {
        munit_assert_true(
            bfilter_query(bf, &values[i], sizeof(int))
        );
    }

    return MUNIT_OK;
}

/* -------------------------------------------------- */

MunitResult t_bloom_clone(const MunitParameter params[], void *fixture) {
    BloomFilter *bf = fixture;

    int x = 7;
    int y = 9;

    bfilter_insert(bf, &x, sizeof(x));

    BloomFilter *clone = bfilter_clone(bf);

    munit_assert_true(bfilter_query(clone, &x, sizeof(x)));
    munit_assert_false(bfilter_query(clone, &y, sizeof(y)));

    bfilter_destroy(clone);

    return MUNIT_OK;
}

/* -------------------------------------------------- */

MunitResult t_bloom_interface(const MunitParameter params[], void *fixture) {
    BloomFilter *bf = fixture;
    const Filter *iface = &bloom_filter_interface;

    int x = 100;
    int y = 200;

    iface->insert(bf, &x, sizeof(x));

    munit_assert_true(iface->query(bf, &x, sizeof(x)));
    munit_assert_false(iface->query(bf, &y, sizeof(y)));

    return MUNIT_OK;
}

/* -------------------------------------------------- */

MunitResult t_bloom_parameters(const MunitParameter params[], void *fixture) {
    BloomFilter *bf = fixture;

    munit_assert_size(bf_get_num_hf(bf), ==, 3);
    munit_assert_size(bf_get_filter_len(bf), ==, 128);

    BitVector *bv = bf_get_filter(bf);
    munit_assert_not_null(bv);

    return MUNIT_OK;
}

/* -------------------------------------------------- */

MunitTest t_bloom[] = {
    {
        "/bloom-empty-query",
        t_bloom_empty_query,
        t_bloom_setup,
        t_bloom_teardown,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/bloom-insert-query",
        t_bloom_insert_query,
        t_bloom_setup,
        t_bloom_teardown,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/bloom-no-false-negatives",
        t_bloom_no_false_negatives,
        t_bloom_setup,
        t_bloom_teardown,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/bloom-clone",
        t_bloom_clone,
        t_bloom_setup,
        t_bloom_teardown,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/bloom-interface",
        t_bloom_interface,
        t_bloom_setup,
        t_bloom_teardown,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/bloom-parameters",
        t_bloom_parameters,
        t_bloom_setup,
        t_bloom_teardown,
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

const MunitSuite t_bloom_suite = {
    "/bloom-tests",
    t_bloom,
    NULL,
    1,
    MUNIT_SUITE_OPTION_NONE,
};
