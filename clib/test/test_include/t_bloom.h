#ifndef TEST_BLOOM_FILTER_H
#define TEST_BLOOM_FILTER_H

#include "munit.h"

void *t_bloom_setup(const MunitParameter params[], void *user_data);
void t_bloom_teardown(void *fixture);

MunitResult t_bloom_empty_query(const MunitParameter params[], void *fixture);
MunitResult t_bloom_insert_query(const MunitParameter params[], void *fixture);
MunitResult t_bloom_no_false_negatives(const MunitParameter params[], void *fixture);
MunitResult t_bloom_clone(const MunitParameter params[], void *fixture);
MunitResult t_bloom_interface(const MunitParameter params[], void *fixture);
MunitResult t_bloom_parameters(const MunitParameter params[], void *fixture);

extern MunitTest t_bloom[];
extern const MunitSuite t_bloom_suite;

#endif
