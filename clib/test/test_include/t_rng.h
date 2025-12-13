#ifndef TEST_RANDOM_GENERATOR_H
#define TEST_RANDOM_GENERATOR_H

#include "munit.h"

void *t_rng_setup(const MunitParameter params[], void *user_data);
void t_rng_teardown(void *fixture);

MunitResult t_rng_int32(const MunitParameter params[], void *fixture);
MunitResult t_rng_int32_bounded(const MunitParameter params[], void *fixture);
MunitResult t_rng_int32_array(const MunitParameter params[], void *fixture);
MunitResult t_rng_unique_array(const MunitParameter params[], void *fixture);
MunitResult t_rng_choice(const MunitParameter params[], void *fixture);
MunitResult t_rng_clone(const MunitParameter params[], void *fixture);
MunitResult t_rng_double_range(const MunitParameter params[], void *fixture);
MunitResult t_rng_interface(const MunitParameter params[], void *fixture);

extern MunitTest t_rng[];
extern const MunitSuite t_rng_suite;

#endif
