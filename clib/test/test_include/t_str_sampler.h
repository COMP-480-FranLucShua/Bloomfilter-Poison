#ifndef TEST_STR_SAMPLER_H
#define TEST_STR_SAMPLER_H

#include "munit.h"

void *t_str_sampler_setup(const MunitParameter params[], void *user_data);
void t_str_sampler_teardown(void *fixture);

MunitResult t_str_sampler_length(const MunitParameter params[], void *fixture);
MunitResult t_str_sampler_single_cycle(const MunitParameter params[], void *fixture);
MunitResult t_str_sampler_restart_cycle(const MunitParameter params[], void *fixture);
MunitResult t_str_sampler_sample_array(const MunitParameter params[], void *fixture);
MunitResult t_str_sampler_interface(const MunitParameter params[], void *fixture);

extern MunitTest t_str_sampler[];
extern const MunitSuite t_str_sampler_suite;

#endif
