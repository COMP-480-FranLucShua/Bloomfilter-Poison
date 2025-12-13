#include "munit.h"
#include "t_bvec.h"
#include "t_hset.h"
#include "t_bloom.h"
#include "t_rng.h"
#include "t_str_sampler.h"
#include "t_kmeans.h"
#include "t_system.h"


static MunitSuite t_suites[] = {
    {NULL, NULL, NULL, 0, 0}, // t_bvec_suite
    {NULL, NULL, NULL, 0, 0}, // t_hset_suite
    {NULL, NULL, NULL, 0, 0}, // t_bloom_suite
    {NULL, NULL, NULL, 0, 0}, // t_rng_suite
    {NULL, NULL, NULL, 0, 0}, // t_str_sampler_suite
    {NULL, NULL, NULL, 0, 0}, // t_kmeans_suite
    {NULL, NULL, NULL, 0, 0}, // t_system_suite
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
    t_suites[2] = t_bloom_suite;
    t_suites[3] = t_rng_suite;
    t_suites[4] = t_str_sampler_suite;
    t_suites[5] = t_kmeans_suite;
    t_suites[6] = t_system_suite;
    
    return munit_suite_main(&t_suite, NULL, argc, argv);
}