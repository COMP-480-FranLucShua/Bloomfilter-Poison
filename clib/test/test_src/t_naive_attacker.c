#include <stdlib.h>
#include <string.h>

#include "t_naive_attacker.h"

#include "naive_attacker.h"
#include "system.h"
#include "str_sampler.h"
#include "rand_generator.h"
#include "hash_set.h"
#include "bloom_filter.h"
#include "murmur_hash.h"

/* -------------------------------------------------- */
/* Setup / Teardown                                   */
/* -------------------------------------------------- */

typedef struct {
    Naive_Attacker *attacker;
    SystemEmulator *system;
} AttackerFixture;

static char *attack_strings[] = {
    "a", "b", "c", "d", "e",
    "f", "g", "h", "i", "j",
};

void *na_rng_inst, *na_set_inst, *na_filter_inst, *na_sampler_inst;

void *t_naive_attacker_setup(const MunitParameter params[], void *user_data) {
    /* RNG */
    na_rng_inst = rng_new(123);

    /* Sampler */
    na_sampler_inst = str_sampler_create(
        attack_strings,
        10,
        &rand_generator_interface,
        na_rng_inst
    );

    /* Set */
    na_set_inst = hset_new(32, 0xdeadbeef, murmur3_32);

    /* Filter */
    na_filter_inst = bfilter_new(128, 3, murmur3_32, &rand_generator_interface, na_rng_inst);

    /* System */
    SystemEmulator *sys = sys_new(
        &hash_set_interface, na_set_inst,
        &bloom_filter_interface, na_filter_inst,
        &rand_generator_interface, na_rng_inst,
        1.0
    );

    /* Attacker */
    Naive_Attacker *attacker = naive_attacker_create(
        &system_emulator_interface, sys,
        &str_sampler_interface, na_sampler_inst
    );

    AttackerFixture *fx = malloc(sizeof(AttackerFixture));
    fx->attacker = attacker;
    fx->system = sys;

    return fx;
}

void t_naive_attacker_teardown(void *fixture) {
    AttackerFixture *fx = fixture;

    naive_attacker_destroy(fx->attacker);
    sys_destroy(fx->system);

    rng_destroy(na_rng_inst);
    hset_destroy(na_set_inst);
    bfilter_destroy(na_filter_inst);
    str_sampler_destroy(na_sampler_inst);

    free(fx);
}

/* -------------------------------------------------- */
/* Attack executes without crashing                   */
/* -------------------------------------------------- */

MunitResult t_naive_attacker_attack_runs(const MunitParameter params[], void *fixture) {
    AttackerFixture *fx = fixture;

    naive_attacker_attack(fx->attacker, 5);

    /* no assertion beyond survival */
    munit_assert_true(true);

    return MUNIT_OK;
}

/* -------------------------------------------------- */
/* attack_size = 0 is a no-op                          */
/* -------------------------------------------------- */

MunitResult t_naive_attacker_zero_attack(const MunitParameter params[], void *fixture) {
    AttackerFixture *fx = fixture;

    size_t before = hash_set_interface.length(fx->system);

    naive_attacker_attack(fx->attacker, 0);

    size_t after = hash_set_interface.length(fx->system);

    munit_assert_size(before, ==, after);

    return MUNIT_OK;
}

/* -------------------------------------------------- */
/* Attacker inserts at most attack_size values         */
/* -------------------------------------------------- */

MunitResult t_naive_attacker_attack_size_bound(const MunitParameter params[], void *fixture) {
    AttackerFixture *fx = fixture;

    size_t attack_size = 6;

    naive_attacker_attack(fx->attacker, attack_size);

    size_t inserted = sys_saturation(fx->system);

    munit_assert_size(inserted, <=, attack_size);

    return MUNIT_OK;
}

/* -------------------------------------------------- */
/* Attacker interface correctness                     */
/* -------------------------------------------------- */

MunitResult t_naive_attacker_interface(const MunitParameter params[], void *fixture) {
    AttackerFixture *fx = fixture;

    const Attacker *iface = &naive_attacker_interface;

    iface->attack(fx->attacker, 4);

    size_t inserted = sys_saturation(fx->system);

    munit_assert_size(inserted, <=, 4);

    return MUNIT_OK;
}

/* -------------------------------------------------- */

MunitTest t_naive_attacker[] = {
    {
        "/naive-attacker-runs",
        t_naive_attacker_attack_runs,
        t_naive_attacker_setup,
        t_naive_attacker_teardown,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/naive-attacker-zero",
        t_naive_attacker_zero_attack,
        t_naive_attacker_setup,
        t_naive_attacker_teardown,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/naive-attacker-size-bound",
        t_naive_attacker_attack_size_bound,
        t_naive_attacker_setup,
        t_naive_attacker_teardown,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/naive-attacker-interface",
        t_naive_attacker_interface,
        t_naive_attacker_setup,
        t_naive_attacker_teardown,
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

const MunitSuite t_naive_attacker_suite = {
    "/naive-attacker-tests",
    t_naive_attacker,
    NULL,
    1,
    MUNIT_SUITE_OPTION_NONE,
};
