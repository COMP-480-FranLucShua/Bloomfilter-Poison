#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "interfaces/attacker.h"
#include "interfaces/filter.h"
#include "interfaces/system.h"
#include "bit_vector.h"

#include "sigma_attacker.h"


void sigma_attacker_attack(void *sa, size_t attack_size);

const Attacker sigma_attacker_interface = {
	.attack = sigma_attacker_attack,
};

struct Sigma_Attacker {
	const System *sys;
	void *sys_inst;
	const Filter *fltr;
	void *fltr_inst;

	size_t step_size; // number of hash functions in bloom filter
};

// static size_t min(size_t a, size_t b) {
// 	if (a > b) {
// 		return b;
// 	}
// 	return a;
// }

Sigma_Attacker *sigma_attacker_create(
	const System *sys, void *sys_inst,
	const Filter *fltr, void *fltr_inst
) {
	Sigma_Attacker *sa = malloc(sizeof(Sigma_Attacker));
	if (!sa) return NULL;

	size_t step_size = fltr->get_num_hf(fltr_inst);

	sa->sys = sys;
	sa->sys_inst = sys_inst;
	sa->fltr = fltr;
	sa->fltr_inst = fltr_inst;
	sa->step_size = step_size;

	return sa;
}

void sigma_attacker_destroy(Sigma_Attacker *self) {
	free(self);
}

/**
 * Simulation of theoretical limit, where each insertion sets 
 * (attack_size * # hash functions) bits in vector
 */
void sigma_attacker_attack(void *self, size_t attack_size) {
	Sigma_Attacker *sa = (Sigma_Attacker *)self;

	BitVector *bvec = sa->fltr->get_filter(sa->fltr_inst);

	size_t starting_sum = bvec_sum(bvec);
	size_t current_sum = starting_sum;

	size_t bit_idx = 0;

	while (current_sum < starting_sum + (attack_size * sa->step_size) && current_sum < bvec_len(bvec)) {
		bvec_set_bit(bvec, bit_idx, 1);
		bit_idx ++;

		current_sum = bvec_sum(bvec);
	}
}

