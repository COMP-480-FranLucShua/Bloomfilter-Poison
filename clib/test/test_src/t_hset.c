#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "t_hset.h"
#include "hash_set.h"
#include "murmur_hash.h"
#include "rand_generator.h"

void *t_hset_setup(const MunitParameter params[], void *user_data) {
    HashSet *set = hset_new(16, 0xdeadbeef, murmur3_32);
    return set;
}

void t_hset_teardown(void *fixture) {
    hset_destroy(fixture);
}

/* -------------------------------------------------- */

MunitResult t_hset_insert_query(const MunitParameter params[], void *fixture) {
    HashSet *set = fixture;

    int a = 10, b = 20, c = 30;

    hset_insert(set, &a, sizeof(a));
    hset_insert(set, &b, sizeof(b));

    munit_assert_true(hset_query(set, &a, sizeof(a)));
    munit_assert_true(hset_query(set, &b, sizeof(b)));
    munit_assert_false(hset_query(set, &c, sizeof(c)));

    return MUNIT_OK;
}

/* -------------------------------------------------- */

MunitResult t_hset_duplicate_insert(const MunitParameter params[], void *fixture) {
    HashSet *set = fixture;

    int x = 42;
    int y = 42;
    int z = 42;

    hset_insert(set, &x, sizeof(x));
    hset_insert(set, &y, sizeof(y));
    hset_insert(set, &z, sizeof(z));


    munit_assert_size(hset_length(set), ==, 1);

    return MUNIT_OK;
}

/* -------------------------------------------------- */

MunitResult t_hset_delete(const MunitParameter params[], void *fixture) {
    HashSet *set = fixture;

    int a = 1, b = 2;

    hset_insert(set, &a, sizeof(a));
    hset_insert(set, &b, sizeof(b));

    munit_assert_size(hset_length(set), ==, 2);

    hset_delete(set, &a, sizeof(a));

    munit_assert_false(hset_query(set, &a, sizeof(a)));
    munit_assert_true(hset_query(set, &b, sizeof(b)));
    munit_assert_size(hset_length(set), ==, 1);

    return MUNIT_OK;
}

/* -------------------------------------------------- */

MunitResult t_hset_length(const MunitParameter params[], void *fixture) {
    HashSet *set = fixture;

    munit_assert_size(hset_length(set), ==, 0);

    int size = 1000;

    int *integers = (int *)calloc(size, sizeof(int));

    RandomGenerator *rng = rng_new(1234577ULL);

    rng_generate_int32_array(rng, integers, size);

    rng_destroy(rng);

    for (int i = 0; i < size; i++) {
        hset_insert(set, &integers[i], sizeof(int));

        munit_assert_size(hset_length(set), ==, (size_t)(i + 1));
    }

    free(integers);


    return MUNIT_OK;
}

/* -------------------------------------------------- */

MunitResult t_hset_clone(const MunitParameter params[], void *fixture) {
    HashSet *set = fixture;

    int vals[5] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; i++) {
        hset_insert(set, &vals[i], sizeof(int));
    }

    HashSet *clone = hset_clone(set);

    munit_assert_size(hset_length(clone), ==, 5);

    for (int i = 0; i < 5; i++) {
        munit_assert_true(hset_query(clone, &vals[i], sizeof(int)));
    }

    hset_destroy(clone);
    return MUNIT_OK;
}

/* -------------------------------------------------- */

MunitResult t_hset_iterator(const MunitParameter params[], void *fixture) {
    HashSet *set = fixture;

    int vals[4] = {7, 8, 9, 10};
    for (int i = 0; i < 4; i++) {
        hset_insert(set, &vals[i], sizeof(int));
    }

    size_t it = 0;
    size_t count = 0;

    void *item;
    while ((item = hset_next(set, &it)) != NULL) {
        int v;
        memcpy(&v, item, sizeof(int));

        bool found = false;
        for (int i = 0; i < 4; i++) {
            if (v == vals[i]) {
                found = true;
                break;
            }
        }

        munit_assert_true(found);
        count++;
    }

    munit_assert_size(count, ==, 4);

    return MUNIT_OK;
}

/* -------------------------------------------------- */

MunitResult t_hset_set_interface(const MunitParameter params[], void *fixture) {
    HashSet *hset = fixture;
    const Set *iface = &hash_set_interface;

    int a = 11, b = 22;

    iface->insert(hset, &a, sizeof(a));
    iface->insert(hset, &b, sizeof(b));

    munit_assert_true(iface->query(hset, &a, sizeof(a)));
    munit_assert_true(iface->query(hset, &b, sizeof(b)));

    iface->remove(hset, &a, sizeof(a));

    munit_assert_false(iface->query(hset, &a, sizeof(a)));
    munit_assert_size(iface->length(hset), ==, 1);

    return MUNIT_OK;
}

/* -------------------------------------------------- */

MunitTest t_hset[] = {
    {
        "/hset-insert-query",
        t_hset_insert_query,
        t_hset_setup,
        t_hset_teardown,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/hset-duplicate-insert",
        t_hset_duplicate_insert,
        t_hset_setup,
        t_hset_teardown,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/hset-delete",
        t_hset_delete,
        t_hset_setup,
        t_hset_teardown,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/hset-length",
        t_hset_length,
        t_hset_setup,
        t_hset_teardown,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/hset-clone",
        t_hset_clone,
        t_hset_setup,
        t_hset_teardown,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/hset-iterator",
        t_hset_iterator,
        t_hset_setup,
        t_hset_teardown,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/hset-set-interface",
        t_hset_set_interface,
        t_hset_setup,
        t_hset_teardown,
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

const MunitSuite t_hset_suite = {
    "/hset-tests",
    t_hset,
    NULL,
    1,
    MUNIT_SUITE_OPTION_NONE,
};
