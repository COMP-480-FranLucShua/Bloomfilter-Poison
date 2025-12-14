#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>

#include "interfaces/attacker.h"
#include "interfaces/system.h"
#include "interfaces/sampler.h"
#include "timing_attacker_st.h"
#include "kmeans1d.h"



void timing_attacker_st_attack(void *ta, size_t attack_size);

static double train(void *self, size_t size);

const Attacker timing_attacker_st_interface = {
	.attack = timing_attacker_st_attack,
};

struct TimingAttackerST {
	const System *sstm;
	void *sstm_inst;

	const Sampler *smplr;
	void *smplr_inst;

	bool is_trained;
	double threshold;
};

TimingAttackerST* timing_attacker_st_create(
	const System *sstm, void *sstm_inst,
	const Sampler *smplr, void *smplr_inst
) {
	TimingAttackerST* ta = malloc(sizeof(TimingAttackerST));
	if (!ta) return NULL;

	ta->sstm = sstm;
	ta->sstm_inst = sstm_inst;

	ta->smplr = smplr;
	ta->smplr_inst = smplr_inst;

	ta->is_trained = false;
	ta->threshold = 0.0;

	return ta;
}

void timing_attacker_st_destroy(TimingAttackerST* ta) {
	free(ta);
}

void timing_attacker_st_attack(void *self, size_t attack_size) {
	if (attack_size == 0)
		return;
	
	TimingAttackerST *ta = (TimingAttackerST *)self;

	if (!ta->is_trained) {
		// train ta

		ta->threshold = train(self, attack_size);
		ta->is_trained = true;

		return;
	}
	// run attack

	double elapsed_seconds;

	void *sample = NULL;
	size_t length;
	size_t current_attack_size = 0;

	// NOTE: sample is owned by smplr_inst
	sample = ta->smplr->sample(ta->smplr_inst, &length);

	// attack until attack size met or entire dataset sampled
	for (size_t i = 0; current_attack_size < attack_size && i < ta->smplr->length(ta->smplr_inst); i++) {

		ta->sstm->query(ta->sstm_inst, sample, length, &elapsed_seconds, NULL);
		if (elapsed_seconds < ta->threshold) {
			// true negative from system, meaning at least one corresponding bit is unset
			ta->sstm->insert(ta->sstm_inst, sample, length);
			current_attack_size ++;
		}

		sample = ta->smplr->sample(ta->smplr_inst, &length);
	}
}


static double train(void *self, size_t insert_size) {
	size_t training_size = insert_size * 2;
	TimingAttackerST *ta = self;

	void **training_set_keys = malloc(sizeof(void *) * training_size);
	size_t *training_set_lens = malloc(sizeof(size_t) * training_size);
	double *delays = malloc(sizeof(double) * training_size);


	ta->smplr->sample_array(ta->smplr_inst, training_set_keys, training_set_lens, training_size);

	// 2. insert training set
	ta->sstm->insert_array(ta->sstm_inst, training_set_keys, training_set_lens, insert_size, false);

	// 3. get delays
	ta->sstm->query_array(ta->sstm_inst, training_set_keys, training_set_lens, training_size, NULL, delays, NULL);

	// 4. run k means
	double threshold = kmeans_threshold_1d(delays, training_size);

	free(delays);
	free(training_set_keys);
	free(training_set_lens);

	return threshold;
}