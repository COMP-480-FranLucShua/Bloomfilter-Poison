#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "t_rng.h"
#include "rand_generator.h"
#include "interfaces/generator.h"

/* -------------------------------------------------- */
/* Setup / Teardown                                   */
/* -------------------------------------------------- */

void *t_rng_setup(const MunitParameter params[], void *user_data) {
    return rng_new(123456789u);
}

void t_rng_teardown(void *fixture) {
    rng_destroy(fixture);
}

/* -------------------------------------------------- */

MunitResult t_rng_int32(const MunitParameter params[], void *fixture) {
    RandomGenerator *rng = fixture;

    uint32_t a = rng_generate_int32(rng);
    uint32_t b = rng_generate_int32(rng);

    munit_assert_uint32(a, !=, b);

    return MUNIT_OK;
}

/* -------------------------------------------------- */

MunitResult t_rng_int32_bounded(const MunitParameter params[], void *fixture) {
    RandomGenerator *rng = fixture;

    size_t bound = 10;

    for (int i = 0; i < 100; i++) {
        uint32_t v = rng_generate_int32_bounded(rng, bound);
        munit_assert_uint32(v, <, bound);
    }

    return MUNIT_OK;
}

/* -------------------------------------------------- */

MunitResult t_rng_int32_array(const MunitParameter params[], void *fixture) {
    RandomGenerator *rng = fixture;

    size_t len = 16;
    uint32_t *arr = calloc(len, sizeof(uint32_t));

    rng_generate_int32_array(rng, arr, len);

    bool any_nonzero = false;
    for (size_t i = 0; i < len; i++) {
        if (arr[i] != 0) {
            any_nonzero = true;
            break;
        }
    }

    munit_assert_true(any_nonzero);

    free(arr);
    return MUNIT_OK;
}

/* -------------------------------------------------- */

MunitResult t_rng_unique_array(const MunitParameter params[], void *fixture) {
    RandomGenerator *rng = fixture;

    size_t len = 20;
    uint32_t *arr = calloc(len, sizeof(uint32_t));

    rng_generate_int32_array_unique(rng, arr, len);

    for (size_t i = 0; i < len; i++) {
        for (size_t j = i + 1; j < len; j++) {
            munit_assert_uint32(arr[i], !=, arr[j]);
        }
    }

    free(arr);
    return MUNIT_OK;
}

/* -------------------------------------------------- */

MunitResult t_rng_choice(const MunitParameter params[], void *fixture) {
    RandomGenerator *rng = fixture;

    size_t range = 50;
    size_t len = 10;

    uint32_t *choices = calloc(len, sizeof(uint32_t));

    rng_generate_choice(rng, choices, range, len);

    for (size_t i = 0; i < len; i++) {
        munit_assert_uint32(choices[i], <, range);
        for (size_t j = i + 1; j < len; j++) {
            munit_assert_uint32(choices[i], !=, choices[j]);
        }
    }

    free(choices);
    return MUNIT_OK;
}

/* -------------------------------------------------- */

MunitResult t_rng_clone(const MunitParameter params[], void *fixture) {
    RandomGenerator *rng = fixture;
    RandomGenerator *clone = rng_clone(rng);

    uint32_t a1 = rng_generate_int32(rng);
    uint32_t a2 = rng_generate_int32(clone);

    /* clones should produce the same sequence */
    munit_assert_uint32(a1, ==, a2);

    rng_destroy(clone);
    return MUNIT_OK;
}

/* -------------------------------------------------- */

MunitResult t_rng_double_range(const MunitParameter params[], void *fixture) {
    RandomGenerator *rng = fixture;

    for (int i = 0; i < 100; i++) {
        double d = rng_generate_double(rng);
        munit_assert_double(d, >=, 0.0);
        munit_assert_double(d, <, 1.0);
    }

    return MUNIT_OK;
}

/* -------------------------------------------------- */

MunitResult t_rng_interface(const MunitParameter params[], void *fixture) {
    RandomGenerator *rng = fixture;
    const Generator *iface = &rand_generator_interface;

    uint32_t a = iface->gen_integer(rng);
    uint32_t b = iface->gen_integer(rng);

    munit_assert_uint32(a, !=, b);

    uint32_t arr[5];
    iface->gen_integers(rng, arr, 5);

    for (int i = 0; i < 5; i++) {
        munit_assert_uint32(arr[i], <=, UINT32_MAX);
    }

    return MUNIT_OK;
}

/* -------------------------------------------------- */

MunitTest t_rng[] = {
    {
        "/rng-int32",
        t_rng_int32,
        t_rng_setup,
        t_rng_teardown,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/rng-int32-bounded",
        t_rng_int32_bounded,
        t_rng_setup,
        t_rng_teardown,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/rng-int32-array",
        t_rng_int32_array,
        t_rng_setup,
        t_rng_teardown,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/rng-unique-array",
        t_rng_unique_array,
        t_rng_setup,
        t_rng_teardown,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/rng-choice",
        t_rng_choice,
        t_rng_setup,
        t_rng_teardown,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/rng-clone",
        t_rng_clone,
        t_rng_setup,
        t_rng_teardown,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/rng-double-range",
        t_rng_double_range,
        t_rng_setup,
        t_rng_teardown,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/rng-interface",
        t_rng_interface,
        t_rng_setup,
        t_rng_teardown,
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

const MunitSuite t_rng_suite = {
    "/rng-tests",
    t_rng,
    NULL,
    1,
    MUNIT_SUITE_OPTION_NONE,
};
