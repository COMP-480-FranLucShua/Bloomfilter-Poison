#include "interfaces/attacker.h"
#include "interfaces/filter.h"
#include "bit_vector.h"

#include "sigma_attacker.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

void sigma_attacker_attack(void *sa);

Attacker sigma_attacker_interface = {
	.attack = sigma_attacker_attack,
};

struct Sigma_Attacker {
	const Filter *fltr;
	void *fltr_inst;

	size_t step_size;
	size_t calls;
};

static size_t min(size_t a, size_t b) {
	if (a > b) {
		return b;
	}
	return a;
}

Sigma_Attacker *sigma_attacker_create(
	const Filter *fltr, void *fltr_inst
) {
	Sigma_Attacker *sa = malloc(sizeof(Sigma_Attacker));
	if (!sa) return NULL;

	size_t step_size = fltr->get_num_hf(fltr_inst);

	sa->fltr = fltr;
	sa->fltr_inst = fltr_inst;
	sa->step_size = step_size;
	sa->calls = 0;

	return sa;
}

void sigma_attacker_destroy(Sigma_Attacker *self) {
	free(self);
}

void sigma_attacker_attack(void *self) {
	size_t starting_idx;
	size_t ending_idx;
	BitVector *bvec = NULL;

	Sigma_Attacker *sa = self;

	starting_idx = sa->calls * sa->step_size;
	ending_idx = min(starting_idx + sa->step_size, sa->fltr->get_filter_len(sa->fltr_inst));
	
	bvec = sa->fltr->get_filter(sa->fltr_inst);
	for (size_t idx = starting_idx; idx < ending_idx; idx++) {
		bvec_set_bit(bvec, idx, 1);
	}

	sa->calls++;
}

