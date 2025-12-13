#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "t_str_sampler.h"
#include "str_sampler.h"
#include "rand_generator.h"
#include "interfaces/sampler.h"

/* -------------------------------------------------- */
/* Test data                                          */
/* -------------------------------------------------- */

static char *test_strings[] = {
    "alpha",
    "beta",
    "gamma",
    "delta",
    "epsilon",
};

static const size_t test_string_count = 5;

static Generator *rng;
static void *rng_ptr;

/* -------------------------------------------------- */
/* Setup / Teardown                                   */
/* -------------------------------------------------- */

void *t_str_sampler_setup(const MunitParameter params[], void *user_data) {
    rng_ptr = rng_new(12345);

    return str_sampler_create(
        test_strings,
        test_string_count,
        &rand_generator_interface,
        rng_ptr
    );
}

void t_str_sampler_teardown(void *fixture) {
    Str_Sampler *sampler = fixture;
    rng_destroy(rng_ptr);
    str_sampler_destroy(sampler);
}

/* -------------------------------------------------- */

MunitResult t_str_sampler_length(const MunitParameter params[], void *fixture) {
    Str_Sampler *sampler = fixture;

    munit_assert_size(
        str_sampler_length(sampler),
        ==,
        test_string_count
    );

    return MUNIT_OK;
}

/* -------------------------------------------------- */
/* Each string appears exactly once per cycle         */
/* -------------------------------------------------- */

MunitResult t_str_sampler_single_cycle(const MunitParameter params[], void *fixture) {
    Str_Sampler *sampler = fixture;

    bool seen[test_string_count];
    memset(seen, false, sizeof(seen));

    for (size_t i = 0; i < test_string_count; i++) {
        size_t len = 0;
        char *s = str_sampler_sample(sampler, &len);

        munit_assert_not_null(s);
        munit_assert_size(len, ==, strlen(s));

        bool found = false;
        for (size_t j = 0; j < test_string_count; j++) {
            if (strcmp(s, test_strings[j]) == 0) {
                munit_assert_false(seen[j]);
                seen[j] = true;
                found = true;
                break;
            }
        }

        munit_assert_true(found);
    }

    return MUNIT_OK;
}

/* -------------------------------------------------- */
/* Sampler restarts after exhausting all strings      */
/* -------------------------------------------------- */

MunitResult t_str_sampler_restart_cycle(const MunitParameter params[], void *fixture) {
    Str_Sampler *sampler = fixture;

    /* exhaust one full cycle */
    for (size_t i = 0; i < test_string_count; i++) {
        str_sampler_sample(sampler, NULL);
    }

    /* second cycle should still yield valid strings */
    for (size_t i = 0; i < test_string_count; i++) {
        size_t len = 0;
        char *s = str_sampler_sample(sampler, &len);

        munit_assert_not_null(s);
        munit_assert_size(len, ==, strlen(s));

        bool found = false;
        for (size_t j = 0; j < test_string_count; j++) {
            if (strcmp(s, test_strings[j]) == 0) {
                found = true;
                break;
            }
        }

        munit_assert_true(found);
    }

    return MUNIT_OK;
}

/* -------------------------------------------------- */
/* Sample array API                                   */
/* -------------------------------------------------- */

MunitResult t_str_sampler_sample_array(const MunitParameter params[], void *fixture) {
    Str_Sampler *sampler = fixture;

    size_t n = test_string_count;
    void **samples = calloc(n, sizeof(void *));
    size_t *lengths = calloc(n, sizeof(size_t));

    str_sampler_sample_array(sampler, samples, lengths, n);

    for (size_t i = 0; i < n; i++) {
        char *s = samples[i];
        munit_assert_not_null(s);
        munit_assert_size(lengths[i], ==, strlen(s));

        bool found = false;
        for (size_t j = 0; j < test_string_count; j++) {
            if (strcmp(s, test_strings[j]) == 0) {
                found = true;
                break;
            }
        }

        munit_assert_true(found);
    }

    free(samples);
    free(lengths);

    return MUNIT_OK;
}

/* -------------------------------------------------- */
/* Sampler interface correctness                     */
/* -------------------------------------------------- */

MunitResult t_str_sampler_interface(const MunitParameter params[], void *fixture) {
    Str_Sampler *sampler = fixture;
    const Sampler *iface = &str_sampler_interface;

    size_t len = 0;
    char *s = iface->sample(sampler, &len);

    munit_assert_not_null(s);
    munit_assert_size(len, ==, strlen(s));

    munit_assert_size(
        iface->length(sampler),
        ==,
        test_string_count
    );

    return MUNIT_OK;
}

/* -------------------------------------------------- */

MunitTest t_str_sampler[] = {
    {
        "/str-sampler-length",
        t_str_sampler_length,
        t_str_sampler_setup,
        t_str_sampler_teardown,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/str-sampler-single-cycle",
        t_str_sampler_single_cycle,
        t_str_sampler_setup,
        t_str_sampler_teardown,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/str-sampler-restart-cycle",
        t_str_sampler_restart_cycle,
        t_str_sampler_setup,
        t_str_sampler_teardown,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/str-sampler-sample-array",
        t_str_sampler_sample_array,
        t_str_sampler_setup,
        t_str_sampler_teardown,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/str-sampler-interface",
        t_str_sampler_interface,
        t_str_sampler_setup,
        t_str_sampler_teardown,
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

const MunitSuite t_str_sampler_suite = {
    "/str-sampler-tests",
    t_str_sampler,
    NULL,
    1,
    MUNIT_SUITE_OPTION_NONE,
};
