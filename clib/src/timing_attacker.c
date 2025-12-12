#include "interfaces/attacker.h"
#include "interfaces/system.h"
#include "interfaces/sampler.h"
#include "timing_attacker.h"

#include <string.h>
#include <sys/time.h>
#include <stdlib.h>

void timing_attacker_attack(void *ta);

const Attacker timing_attacker_interface = {
	.attack = timing_attacker_attack,
};

struct TimingAttacker {
	const System *sstm;
	void *sstm_inst;

	const Sampler *smplr;
	void *smplr_inst;

	double time_threshold;
};

TimingAttacker* timing_attacker_create(
	const System *sstm, void *sstm_inst,
	const Sampler *smplr, void *smplr_inst,
	double timer_threshold
) {
	TimingAttacker* ta = malloc(sizeof(TimingAttacker));
	if (!ta) return NULL;

	ta->sstm = sstm;
	ta->sstm_inst = sstm_inst;

	ta->smplr = smplr;
	ta->smplr_inst = smplr_inst;

	ta->time_threshold = timer_threshold;

	return ta;
}

void timing_attacker_destroy(TimingAttacker* ta) {
	free(ta);
}

void timing_attacker_attack(void *self) {
	TimingAttacker *ta = self;

	for (;;) {
		double elapsed_seconds;

		char *sample = NULL;
		size_t length = 0;

		// NOTE: sample is owned by smplr_inst
		ta->smplr->sample(ta->smplr_inst, sample, &length);

		if (sample == NULL) {
			break; //FIXME: Should probably error
		}

		ta->sstm->query(ta->sstm_inst, sample, length, &elapsed_seconds, NULL);
		if (elapsed_seconds > ta->time_threshold) {
			ta->sstm->insert(ta->sstm_inst, sample, length);
			break;
		}
	}
}
