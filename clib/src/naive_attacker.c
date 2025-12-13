#include "interfaces/system.h"
#include "interfaces/sampler.h"
#include "interfaces/attacker.h"

#include "naive_attacker.h"

#include <stdlib.h>
#include <stdio.h>

void naive_attacker_attack(void *na, size_t attack_size);

const Attacker naive_attacker_interface = {
	.attack = naive_attacker_attack,
};

struct Naive_Attacker {
	const System *sstm;
	void *sstm_inst;

	const Sampler *smplr;
	void *smplr_inst;

	double time_threshold;
};

Naive_Attacker *naive_attacker_create(
	const System *sstm, void *sstm_inst,
	const Sampler *smplr, void *smplr_inst
) {
	Naive_Attacker* na = malloc(sizeof(Naive_Attacker));
	if (!na) return NULL;

	na->sstm = sstm;
	na->sstm_inst = sstm_inst;

	na->smplr = smplr;
	na->smplr_inst = smplr_inst;

	na->time_threshold = 0;

	return na;
}

void naive_attacker_destroy(Naive_Attacker *self) {
	free(self);
}

void naive_attacker_attack(void *self, size_t attack_size) {
	Naive_Attacker *na = self;

	char *sample;
	size_t length;

	for (size_t i = 0; i < attack_size; i++) {
		// NOTE: sample is owned by smplr_inst
		sample = na->smplr->sample(na->smplr_inst, &length);
	
		na->sstm->insert(na->sstm_inst, sample, length);
	}
}
