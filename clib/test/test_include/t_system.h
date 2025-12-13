#ifndef TEST_SYSTEM_EMULATOR_H
#define TEST_SYSTEM_EMULATOR_H

#include "munit.h"

void *t_system_setup(const MunitParameter params[], void *user_data);
void t_system_teardown(void *fixture);

MunitResult t_system_insert_query(const MunitParameter params[], void *fixture);
MunitResult t_system_no_false_negatives(const MunitParameter params[], void *fixture);
MunitResult t_system_delay(const MunitParameter params[], void *fixture);
MunitResult t_system_array_api(const MunitParameter params[], void *fixture);
MunitResult t_system_interface(const MunitParameter params[], void *fixture);

extern MunitTest t_system[];
extern const MunitSuite t_system_suite;

#endif
