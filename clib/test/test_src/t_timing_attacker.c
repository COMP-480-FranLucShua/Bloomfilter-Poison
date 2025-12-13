#include <stdlib.h>
#include <string.h>

#include "t_timing_attacker.h"

#include "interfaces/attacker.h"
#include "timing_attacker.h"
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
    TimingAttacker *attacker;
    SystemEmulator *system;
} AttackerFixture;

static char *attack_strings[] = {
    "a", "b", "c", "d", "e",
    "f", "g", "h", "i", "j",
    "k", "l", "m", "n", "o",
    "p", "q", "r", "s", "t",
    "u", "v", "w", "x", "y",
    "a", "b", "c", "d", "e",
    "f", "g", "h", "i", "j",
    "k", "l", "m", "n", "o",
    "p", "q", "r", "s", "t",
    "u", "v", "w", "x", "y",
    "a", "b", "c", "d", "e",
    "f", "g", "h", "i", "j",
    "k", "l", "m", "n", "o",
    "p", "q", "r", "s", "t",
    "u", "v", "w", "x", "y"
};

void *ta_rng_inst, *ta_set_inst, *ta_filter_inst, *ta_sampler_inst;

void *t_timing_attacker_setup(const MunitParameter params[], void *user_data) {
    /* RNG */
    ta_rng_inst = rng_new(123);

    /* Sampler */
    ta_sampler_inst = str_sampler_create(
        attack_strings,
        75,
        &rand_generator_interface,
        ta_rng_inst
    );

    /* Set */
    ta_set_inst = hset_new(32, 0xdeadbeef, murmur3_32);

    /* Filter */
    ta_filter_inst = bfilter_new(
        128,
        3,
        murmur3_32,
        &rand_generator_interface,
        ta_rng_inst
    );

    /* System */
    SystemEmulator *sys = sys_new(
        &hash_set_interface, ta_set_inst,
        &bloom_filter_interface, ta_filter_inst,
        &rand_generator_interface, ta_rng_inst,
        1.0
    );

    /* Attacker */
    TimingAttacker *attacker = timing_attacker_create(
        &system_emulator_interface, sys,
        &str_sampler_interface, ta_sampler_inst,
        0.5   /* training proportion */
    );

    AttackerFixture *fx = malloc(sizeof(AttackerFixture));
    fx->attacker = attacker;
    fx->system = sys;

    return fx;
}

void t_timing_attacker_teardown(void *fixture) {
    AttackerFixture *fx = fixture;

    timing_attacker_destroy(fx->attacker);
    sys_destroy(fx->system);

    rng_destroy(ta_rng_inst);
    hset_destroy(ta_set_inst);
    bfilter_destroy(ta_filter_inst);
    str_sampler_destroy(ta_sampler_inst);

    free(fx);
}

/* -------------------------------------------------- */
/* Attack executes without crashing                   */
/* -------------------------------------------------- */

MunitResult t_timing_attacker_attack_runs(const MunitParameter params[], void *fixture) {
    AttackerFixture *fx = fixture;

    timing_attacker_attack(fx->attacker, 5);

    /* no assertion beyond survival */
    munit_assert_true(true);

    return MUNIT_OK;
}

/* -------------------------------------------------- */
/* attack_size = 0 is a no-op                          */
/* -------------------------------------------------- */

MunitResult t_timing_attacker_zero_attack(const MunitParameter params[], void *fixture) {
    AttackerFixture *fx = fixture;

    size_t before = sys_saturation(fx->system);

    timing_attacker_attack(fx->attacker, 0);

    size_t after = sys_saturation(fx->system);

    munit_assert_size(before, ==, after);

    return MUNIT_OK;
}

/* -------------------------------------------------- */
/* Attacker inserts at most attack_size values         */
/* -------------------------------------------------- */

MunitResult t_timing_attacker_attack_size_bound(const MunitParameter params[], void *fixture) {
    AttackerFixture *fx = fixture;

    size_t attack_size = 6;

    timing_attacker_attack(fx->attacker, attack_size);

    size_t inserted = sys_saturation(fx->system);

    munit_assert_size(inserted, <=, attack_size);

    return MUNIT_OK;
}

/* -------------------------------------------------- */
/* Attacker interface correctness                     */
/* -------------------------------------------------- */

MunitResult t_timing_attacker_interface(const MunitParameter params[], void *fixture) {
    AttackerFixture *fx = fixture;

    const Attacker *iface = &timing_attacker_interface;

    iface->attack(fx->attacker, 4);

    size_t inserted = sys_saturation(fx->system);

    munit_assert_size(inserted, <=, 4);

    return MUNIT_OK;
}

/* -------------------------------------------------- */

MunitTest t_timing_attacker[] = {
    {
        "/timing-attacker-runs",
        t_timing_attacker_attack_runs,
        t_timing_attacker_setup,
        t_timing_attacker_teardown,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/timing-attacker-zero",
        t_timing_attacker_zero_attack,
        t_timing_attacker_setup,
        t_timing_attacker_teardown,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/timing-attacker-size-bound",
        t_timing_attacker_attack_size_bound,
        t_timing_attacker_setup,
        t_timing_attacker_teardown,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/timing-attacker-interface",
        t_timing_attacker_interface,
        t_timing_attacker_setup,
        t_timing_attacker_teardown,
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

const MunitSuite t_timing_attacker_suite = {
    "/timing-attacker-tests",
    t_timing_attacker,
    NULL,
    1,
    MUNIT_SUITE_OPTION_NONE,
};
