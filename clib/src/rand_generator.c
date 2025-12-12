#include <stdlib.h>
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "pcg/pcg_basic.h"
#include "rand_generator.h"
#include "murmur_hash.h"
#include "hash_set.h"

struct RandomGenerator {
    pcg32_random_t pcg;
    double gamma_alpha;
    double gamma_theta;
};

// *** CONSTRUCTOR/DESTRUCTOR
RandomGenerator *rng_new(uint32_t seed) {
    RandomGenerator *rng = (RandomGenerator *)malloc(sizeof(RandomGenerator));

    const uint64_t default_state[] = PCG32_INITIALIZER;

    pcg32_srandom_r(&(rng->pcg), default_state[0], seed);

    rng->gamma_alpha = 1.0;
    rng->gamma_theta = 1.0;

    return rng;
}

void *rng_destroy(RandomGenerator *rng) {
    if (rng != NULL) {
        free(rng);
        rng = NULL;
    }
    return (void *)rng;
}

RandomGenerator *rng_clone(RandomGenerator *rng) {
    RandomGenerator *rng_new = (RandomGenerator *)malloc(sizeof(RandomGenerator));

    rng_new->pcg.state = rng->pcg.state;
    rng_new->pcg.inc = rng->pcg.inc;

    return rng_new;
}

// *** SETTER
void rng_set_gamma_params(RandomGenerator *rng, double alpha, double theta) {
    rng->gamma_alpha = alpha;
    rng->gamma_theta = theta;
}

// *** VALUE GENERATOR FUNCTIONS

uint32_t rng_generate_int32(RandomGenerator *rng) {
    return pcg32_random_r(&(rng->pcg));
}

uint32_t rng_generate_int32_bounded(RandomGenerator *rng, size_t bound) {
    return pcg32_boundedrand_r(&(rng->pcg), bound);
}

double rng_generate_double(RandomGenerator *rng) {
    uint64_t high = rng_generate_int32(rng) >> 5; // higher 27 bits of mantissa;
    uint64_t low = rng_generate_int32(rng) >> 6; // lower 26 bits of mantissa;

    uint64_t bits = (high << 26) | low;

    return bits * (1.0 / 9007199254740992.0); // divide by 2^53
}


uint32_t * rng_generate_int32_array(RandomGenerator *rng, uint32_t *array, size_t len) {
    for (size_t i = 0; i < len; i++) {
        array[i] = rng_generate_int32(rng);
    }
    return array;
}
uint32_t * rng_generate_int32_bounded_array(RandomGenerator *rng, size_t bound, uint32_t *array, size_t len) {
    for (size_t i = 0; i < len; i++) {
        array[i] = rng_generate_int32_bounded(rng, bound);
    }
    return array;
}
double * rng_generate_double_array(RandomGenerator *rng, double *array, size_t len) {
    for (size_t i = 0; i < len; i++) {
        array[i] = rng_generate_double(rng);
    }
    return array;
}

// *** RANDOM SELECTION

uint32_t * rng_generate_choice(RandomGenerator *rng, size_t range, uint32_t *array, size_t len) {
    if (range < len) {
        range = len;
    }

    // Create pool array
    uint32_t *pool = (uint32_t *)malloc(range * sizeof(uint32_t));
    for (size_t i = 0; i < range; i++) {
        pool[i] = i;
    }

    // Partial fisher yates shuffling
    for (size_t i = 0; i < len; i++) {
        // Pick j in [i, range)
        uint32_t j = rng_generate_int32_bounded(rng, range-i) + i;

        // swap pool[i] and pool[j]
        uint32_t temp = pool[j];
        pool[j] = pool[i];
        pool[i] = temp;

        // save result
        array[i] = pool[i];
    }

    return array;
}

uint32_t * rng_generate_int32_array_unique(RandomGenerator *rng, uint32_t *array, size_t len) {
    HashSet *set = hset_new(256, 0xDEADBEEF, murmur3_32);

    for (size_t i = 0; i < len; i++) {
        uint32_t num = rng_generate_int32(rng);
        while (hset_query(set, num)) {
            num = rng_generate_int32(rng);
        }
        hset_insert(num);
        array[i] = num;
    }

    return array;
}

// *** RANDOM DISTRIBUTIONS

double generate_standard_normal(RandomGenerator *rng) {
    double u1 = rng_generate_double(rng);
    double u2 = rng_generate_double(rng);

    return sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
}

double gamma_mt(RandomGenerator *rng, double alpha) {
    double d = alpha - 1.0/3.0;
    double c = 1.0 / sqrt(9.0 * d);

    while (true)
    {
        double x = generate_standard_normal(rng);
        double v = 1.0 + c * x;
        if (v <= 0.0) continue;

        v = v * v * v;
        double u = rng_generate_double(rng);

        if (u < 1.0 - 0.0331 * (x * x) * (x * x))
            return d * v;

        if (log(u) < 0.5 * x * x + d * (1.0 - v + log(v)))
            return d * v;
    }
}

double gamma_mt_small(RandomGenerator *rng, double alpha)
{
    double u = rng_generate_double(rng);
    return gamma_mt(rng, alpha + 1.0) * pow(u, 1.0 / alpha);
}

double rng_generate_gamma(RandomGenerator *rng) {
    return rng_generate_gamma_params(rng, rng->gamma_alpha, rng->gamma_theta);
}

double rng_generate_gamma_params(RandomGenerator *rng, double alpha, double theta) {
    double g = (alpha < 1.0)
        ? gamma_mt_small(rng, alpha)
        : gamma_mt(rng, alpha);

    return g * theta;
}