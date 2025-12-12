#ifndef TEST_HASH_SET_H
#define TEST_HASH_SET_H

#include <stdbool.h>
#include "munit.h"

void *t_hset_setup(const MunitParameter params[], void *user_data);
void t_hset_teardown(void *fixture);

MunitResult t_hset_insert_query(const MunitParameter params[], void *fixture);
MunitResult t_hset_duplicate_insert(const MunitParameter params[], void *fixture);
MunitResult t_hset_delete(const MunitParameter params[], void *fixture);
MunitResult t_hset_length(const MunitParameter params[], void *fixture);
MunitResult t_hset_clone(const MunitParameter params[], void *fixture);
MunitResult t_hset_iterator(const MunitParameter params[], void *fixture);
MunitResult t_hset_set_interface(const MunitParameter params[], void *fixture);

extern MunitTest t_hset[];
extern const MunitSuite t_hset_suite;

#endif
