#include "interfaces/system.h"
#include "interfaces/sampler.h"
#include "interfaces/attacker.h"

#include "naive_attacker.h"

#include <stdlib.h>

void naive_attacker_attack(void *na);

Attacker naive_attacker_interface = {
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

	return na;
}

void naive_attacker_destroy(Naive_Attacker *self) {
	free(self);
}

void naive_attacker_attack(void *self) {
	Naive_Attacker *na = self;

	char *sample = NULL;
	size_t length = 0;

	// NOTE: sample is owned by smplr_inst
	na->smplr->sample(na->smplr_inst, sample, &length);

	if (sample == NULL) {
		return; //FIXME: Should probably error
	}

	na->sstm->insert(na->sstm_inst, sample, length);
}
