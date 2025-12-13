#ifndef TEST_TIMING_ATTACKER_H
#define TEST_TIMING_ATTACKER_H

#include "munit.h"

void *t_timing_attacker_setup(const MunitParameter params[], void *user_data);
void t_timing_attacker_teardown(void *fixture);

MunitResult t_timing_attacker_attack_runs(const MunitParameter params[], void *fixture);
MunitResult t_timing_attacker_zero_attack(const MunitParameter params[], void *fixture);
MunitResult t_timing_attacker_attack_size_bound(const MunitParameter params[], void *fixture);
MunitResult t_timing_attacker_interface(const MunitParameter params[], void *fixture);

extern MunitTest t_timing_attacker[];
extern const MunitSuite t_timing_attacker_suite;

#endif
