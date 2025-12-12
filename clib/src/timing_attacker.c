#include <string.h>
#include <sys/time.h>
#include <stdlib.h>

#include "interfaces/attacker.h"
#include "interfaces/system.h"
#include "interfaces/sampler.h"
#include "timing_attacker.h"
#include "kmeans1d.h"



void timing_attacker_attack(void *ta, size_t attack_size);

static double train(void *self, size_t training_size, size_t insert_size);

const Attacker timing_attacker_interface = {
	.attack = timing_attacker_attack,
};

struct TimingAttacker {
	const System *sstm;
	void *sstm_inst;

	const Sampler *smplr;
	void *smplr_inst;

	double training_proportion;
};

TimingAttacker* timing_attacker_create(
	const System *sstm, void *sstm_inst,
	const Sampler *smplr, void *smplr_inst,
	double training_proportion
) {
	TimingAttacker* ta = malloc(sizeof(TimingAttacker));
	if (!ta) return NULL;

	ta->sstm = sstm;
	ta->sstm_inst = sstm_inst;

	ta->smplr = smplr;
	ta->smplr_inst = smplr_inst;

	ta->training_proportion = training_proportion;

	return ta;
}

void timing_attacker_destroy(TimingAttacker* ta) {
	free(ta);
}

void timing_attacker_attack(void *self, size_t attack_size) {
	TimingAttacker *ta = (TimingAttacker *)self;

	// train attacker
	size_t training_set_size = (size_t)(attack_size * ta->training_proportion);
	size_t insert_size = training_set_size / 2;

	double threshold = train(ta, training_set_size, insert_size);

	size_t current_attack_size = insert_size;

	double elapsed_seconds;

	void *sample = NULL;
	size_t length;

	// NOTE: sample is owned by smplr_inst
	ta->smplr->sample(ta->smplr_inst, &length);

	// attack until attack size met or entire dataset sampled
	for (size_t i = 0; current_attack_size < attack_size && i < ta->smplr->length(ta->smplr_inst); i++) {

		ta->sstm->query(ta->sstm_inst, sample, length, &elapsed_seconds, NULL);
		if (elapsed_seconds < threshold) {
			// true negative from system, meaning at least one corresponding bit is unset
			ta->sstm->insert(ta->sstm_inst, sample, length);
			current_attack_size ++;
		}
	}
}


static double train(void *self, size_t training_size, size_t insert_size) {
	TimingAttacker *ta = self;

	void **training_set_keys = malloc(sizeof(void *) * training_size);
	size_t *training_set_lens = malloc(sizeof(size_t) * training_size);
	double *delays = malloc(sizeof(double) * training_size);


	ta->smplr->sample_array(ta->smplr_inst, training_set_keys, training_set_lens, training_size);

	// 2. insert training set
	ta->sstm->insert_array(ta->sstm_inst, training_set_keys, training_set_lens, insert_size);

	// 3. get delays
	ta->sstm->query_array(ta->sstm_inst, training_set_keys, training_set_lens, training_size, NULL, delays, NULL);

	// 4. run k means
	double threshold = kmeans_threshold_1d(delays, training_size);

	free(delays);
	free(training_set_keys);
	free(training_set_lens);

	return threshold;
}