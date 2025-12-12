#include "munit.h"
#include "t_bvec.h"
#include "t_hset.h"


static MunitSuite t_suites[] = {
    {NULL, NULL, NULL, 0, 0}, // t_bvec_suite
    {NULL, NULL, NULL, 0, 0}, // t_bvec_suite
    {NULL, NULL, NULL, 0, 0},
    // NULL,
};


static const MunitSuite t_suite = {
    "",
    NULL,
    t_suites,
    10,
    MUNIT_SUITE_OPTION_NONE,
};

int main(int argc, char * argv[]) {
    t_suites[0] = t_bvec_suite;
    t_suites[1] = t_hset_suite;
    
    return munit_suite_main(&t_suite, NULL, argc, argv);
}