#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "t_kmeans.h"
#include "kmeans1d.h"

/* -------------------------------------------------- */
/* Two well-separated clusters                       */
/* -------------------------------------------------- */

MunitResult t_kmeans_simple_split(const MunitParameter params[], void *user_data) {
    double data[] = { 1, 2, 3, 10, 11, 12 };
    size_t n = sizeof(data) / sizeof(data[0]);

    double t = kmeans_threshold_1d(data, n);

    /* threshold should lie between clusters */
    munit_assert_double(t, >, 3.0);
    munit_assert_double(t, <, 10.0);

    return MUNIT_OK;
}

/* -------------------------------------------------- */
/* Symmetric clusters → midpoint                     */
/* -------------------------------------------------- */

MunitResult t_kmeans_symmetric(const MunitParameter params[], void *user_data) {
    double data[] = { -10, -8, -6, 6, 8, 10 };
    size_t n = sizeof(data) / sizeof(data[0]);

    double t = kmeans_threshold_1d(data, n);

    /* symmetric around zero */
    munit_assert_double(fabs(t), <, 1e-6);

    return MUNIT_OK;
}

/* -------------------------------------------------- */
/* Input order must not matter                        */
/* -------------------------------------------------- */

MunitResult t_kmeans_order_invariant(const MunitParameter params[], void *user_data) {
    double data1[] = { 1, 2, 3, 10, 11, 12 };
    double data2[] = { 12, 3, 10, 1, 11, 2 };

    double t1 = kmeans_threshold_1d(data1, 6);
    double t2 = kmeans_threshold_1d(data2, 6);

    munit_assert_double(fabs(t1 - t2), <, 1e-6);

    return MUNIT_OK;
}

/* -------------------------------------------------- */
/* Single cluster input                               */
/* -------------------------------------------------- */

MunitResult t_kmeans_single_cluster(const MunitParameter params[], void *user_data) {
    double data[] = { 5, 6, 7, 8, 9 };
    size_t n = sizeof(data) / sizeof(data[0]);

    double t = kmeans_threshold_1d(data, n);

    /* must lie within data range */
    munit_assert_double(t, >=, 5.0);
    munit_assert_double(t, <=, 9.0);

    return MUNIT_OK;
}

// /* -------------------------------------------------- */
// /* All values identical                               */
// /* -------------------------------------------------- */

// MunitResult t_kmeans_degenerate(const MunitParameter params[], void *user_data) {
//     double data[] = { 4, 4, 4, 4, 4 };
//     size_t n = sizeof(data) / sizeof(data[0]);

//     double t = kmeans_threshold_1d(data, n);

//     munit_assert_double(t, ==, 4.0);

//     return MUNIT_OK;
// }

/* -------------------------------------------------- */

MunitTest t_kmeans[] = {
    {
        "/kmeans-simple-split",
        t_kmeans_simple_split,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/kmeans-symmetric",
        t_kmeans_symmetric,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/kmeans-order-invariant",
        t_kmeans_order_invariant,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/kmeans-single-cluster",
        t_kmeans_single_cluster,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    // {
    //     "/kmeans-degenerate",
    //     t_kmeans_degenerate,
    //     NULL,
    //     NULL,
    //     MUNIT_TEST_OPTION_NONE,
    //     NULL,
    // },
    {
        NULL,
        NULL,
        NULL,
        NULL,
        0,
        NULL,
    },
};

const MunitSuite t_kmeans_suite = {
    "/kmeans-tests",
    t_kmeans,
    NULL,
    1,
    MUNIT_SUITE_OPTION_NONE,
};
