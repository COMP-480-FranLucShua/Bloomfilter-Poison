#ifndef TEST_NAIVE_ATTACKER_H
#define TEST_NAIVE_ATTACKER_H

#include "munit.h"

void *t_naive_attacker_setup(const MunitParameter params[], void *user_data);
void t_naive_attacker_teardown(void *fixture);

MunitResult t_naive_attacker_attack_runs(const MunitParameter params[], void *fixture);
MunitResult t_naive_attacker_zero_attack(const MunitParameter params[], void *fixture);
MunitResult t_naive_attacker_attack_size_bound(const MunitParameter params[], void *fixture);
MunitResult t_naive_attacker_interface(const MunitParameter params[], void *fixture);

extern MunitTest t_naive_attacker[];
extern const MunitSuite t_naive_attacker_suite;

#endif
