#ifndef TEST_BIT_VECTOR_H
#define TEST_BIT_VECTOR_H
#include <stdbool.h>
#include "munit.h"

void *t_bvec_setup(const MunitParameter params[], void *user_data);
void t_bvec_teardown(void *fixture);

MunitResult t_bvec_fill(const MunitParameter params[], void *fixture);
MunitResult t_bvec_get_set_bit(const MunitParameter params[], void *fixture);
MunitResult t_bvec_len(const MunitParameter params[], void *fixture);
MunitResult t_bvec_sum(const MunitParameter params[], void *fixture);

extern MunitTest t_bvec[];

extern const MunitSuite t_bvec_suite;

#endif