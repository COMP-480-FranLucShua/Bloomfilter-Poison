#ifndef TEST_KMEANS_1D_H
#define TEST_KMEANS_1D_H

#include "munit.h"

MunitResult t_kmeans_simple_split(const MunitParameter params[], void *user_data);
MunitResult t_kmeans_symmetric(const MunitParameter params[], void *user_data);
MunitResult t_kmeans_order_invariant(const MunitParameter params[], void *user_data);
MunitResult t_kmeans_single_cluster(const MunitParameter params[], void *user_data);
MunitResult t_kmeans_degenerate(const MunitParameter params[], void *user_data);

extern MunitTest t_kmeans[];
extern const MunitSuite t_kmeans_suite;

#endif
