#include <stdlib.h>
#include <string.h>

#include "t_system.h"
#include "system.h"

#include "hash_set.h"
#include "bloom_filter.h"
#include "rand_generator.h"
#include "murmur_hash.h"
#include "time.h"

/* -------------------------------------------------- */
/* Setup / Teardown                                   */
/* -------------------------------------------------- */

void *set_inst, *rng_inst, *filter_inst;

void *t_system_setup(const MunitParameter params[], void *user_data) {
    /* --- RNG --- */
    rng_inst = rng_new((uint32_t)time(NULL));

    /* --- Set --- */
    set_inst = hset_new(32, 0xdeadbeef, murmur3_32);

    /* --- Filter --- */
    filter_inst = bfilter_new(
        256,        /* bit vector size */
        3,          /* hash functions */
        murmur3_32,
        &rand_generator_interface,
        rng_inst
    );

    /* --- System --- */
    double delay = 5.0;

    return sys_new(
        &hash_set_interface, set_inst,
        &bloom_filter_interface, filter_inst,
        &rand_generator_interface, rng_inst,
        delay
    );
}

void t_system_teardown(void *fixture) {
    sys_destroy(fixture);
    rng_destroy(rng_inst);
    hset_destroy(set_inst);
    bfilter_destroy(filter_inst);
}

/* -------------------------------------------------- */
/* Basic insert/query                                 */
/* -------------------------------------------------- */

MunitResult t_system_insert_query(const MunitParameter params[], void *fixture) {
    SystemEmulator *sys = fixture;

    int x = 42;
    double delay = 0.0;
    bool fp = true;

    sys_insert(sys, &x, sizeof(x));

    bool result = sys_query(sys, &x, sizeof(x), &delay, &fp);

    munit_assert_true(result);
    munit_assert_false(fp);

    return MUNIT_OK;
}

/* -------------------------------------------------- */
/* No false negatives                                 */
/* -------------------------------------------------- */

MunitResult t_system_no_false_negatives(const MunitParameter params[], void *fixture) {
    SystemEmulator *sys = fixture;

    int values[20];

    for (int i = 0; i < 20; i++) {
        values[i] = i;
        sys_insert(sys, &values[i], sizeof(int));
    }

    for (int i = 0; i < 20; i++) {
        bool fp = true;
        bool result = sys_query(sys, &values[i], sizeof(int), NULL, &fp);

        munit_assert_true(result);
        munit_assert_false(fp);
    }

    return MUNIT_OK;
}

/* -------------------------------------------------- */
/* Delay propagation                                  */
/* -------------------------------------------------- */

MunitResult t_system_delay(const MunitParameter params[], void *fixture) {
    SystemEmulator *sys = fixture;

    int x = 7;
    double delay = 0.0;
    bool fp = false;

    sys_insert(sys, &x, sizeof(x));

    sys_query(sys, &x, sizeof(x), &delay, &fp);

    munit_assert_double(delay, >=, 5.0);

    int y = 100123;
    double delay_2 = 0.0;
    bool fp_2 = false;
    bool result = sys_query(sys, &y, sizeof(y), &delay_2, &fp_2);


    if (result) {
        munit_assert_true(fp_2);
    }
    else {
        munit_assert_double(delay_2, <, delay);
    }
    
    return MUNIT_OK;
}

/* -------------------------------------------------- */
/* Array APIs                                         */
/* -------------------------------------------------- */

MunitResult t_system_array_api(const MunitParameter params[], void *fixture) {
    SystemEmulator *sys = fixture;

    const size_t n = 10;
    int values[n];
    void *ptrs[n];
    size_t lens[n];

    for (size_t i = 0; i < n; i++) {
        values[i] = (int)i;
        ptrs[i] = &values[i];
        lens[i] = sizeof(int);
    }

    sys_insert_array(sys, ptrs, lens, n, false);

    bool results[n];
    double delays[n];
    size_t fp_count = 0;

    sys_query_array(
        sys,
        ptrs,
        lens,
        n,
        results,
        delays,
        &fp_count
    );

    for (size_t i = 0; i < n; i++) {
        munit_assert_true(results[i]);
        munit_assert_double(delays[i], >=, 5.0);
    }

    munit_assert_size(fp_count, ==, 0);

    return MUNIT_OK;
}

/* -------------------------------------------------- */
/* System interface                                   */
/* -------------------------------------------------- */

MunitResult t_system_interface(const MunitParameter params[], void *fixture) {
    SystemEmulator *sys = fixture;
    const System *iface = &system_emulator_interface;

    int x = 99;
    bool fp = true;
    double delay = 0.0;

    iface->insert(sys, &x, sizeof(x));

    bool result = iface->query(sys, &x, sizeof(x), &delay, &fp);

    munit_assert_true(result);
    munit_assert_false(fp);
    munit_assert_double(delay, >=, 5.0);

    return MUNIT_OK;
}

/* -------------------------------------------------- */

MunitTest t_system[] = {
    {
        "/system-insert-query",
        t_system_insert_query,
        t_system_setup,
        t_system_teardown,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/system-no-false-negatives",
        t_system_no_false_negatives,
        t_system_setup,
        t_system_teardown,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/system-delay",
        t_system_delay,
        t_system_setup,
        t_system_teardown,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/system-array-api",
        t_system_array_api,
        t_system_setup,
        t_system_teardown,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/system-interface",
        t_system_interface,
        t_system_setup,
        t_system_teardown,
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

const MunitSuite t_system_suite = {
    "/system-tests",
    t_system,
    NULL,
    1,
    MUNIT_SUITE_OPTION_NONE,
};
