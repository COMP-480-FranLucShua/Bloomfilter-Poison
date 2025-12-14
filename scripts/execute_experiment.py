import numpy as np
from pathlib import Path
import matplotlib.pyplot as plt
from yaspin import yaspin
from tqdm import tqdm

from typing import List, Dict, Tuple, Any

from c_modules import (
    SystemEmulator, 
    BloomFilter, 
    HashSet, 
    RandomNumberGenerator,
    StringArray,
    StringSampler,
    NaiveAttacker,
    TimingAttacker,
    TimingAttackerST,
    SigmaAttacker,
)

from yaml_parser.loader import load_yaml
from yaml_parser.types import AttackerType
import pandas as pd
import sys

yaml_file_path = 'config.yaml'

# ------------------------------------ UTILITIES ------------------------------------ 

@yaspin(text="Loading dataset...")
def load_dataset(path: str):
    data = pd.read_csv(path, sep="\t")
    urllist = data.ClickURL.dropna().unique() # NOTE: 380k items
    return urllist

def run_pipeline(system, test_sample: StringArray, attacker, attack_size: int):
    attacker.attack(attack_size)
    return system.query_array(test_sample)

def assemble_system(buckets: int, num_hfs: int, seed: int, system_delay: float, rng: RandomNumberGenerator, initial_insert: StringArray) -> SystemEmulator:
    bf: BloomFilter = BloomFilter(buckets, num_hfs, rng)
    system: SystemEmulator = SystemEmulator(HashSet(1024, seed), bf, rng, system_delay)
    system.insert_array(initial_insert)
    return system

def assemble_naive_attacker(sys: SystemEmulator, rng: RandomNumberGenerator, dataset: StringArray) -> NaiveAttacker:
    sampler: StringSampler = StringSampler(dataset, rng)
    attacker: NaiveAttacker = NaiveAttacker(sys, sampler)
    return attacker

def assemble_timing_attacker(sys: SystemEmulator, rng: RandomNumberGenerator, dataset: StringArray, training_proportion: float = 0.15) -> TimingAttacker:
    sampler: StringSampler = StringSampler(dataset, rng)
    attacker: TimingAttacker = TimingAttacker(sys, sampler, training_proportion)
    return attacker

def assemble_timing_attacker_st(sys: SystemEmulator, rng: RandomNumberGenerator, dataset: StringArray) -> TimingAttackerST:
    sampler: StringSampler = StringSampler(dataset, rng)
    attacker: TimingAttackerST = TimingAttackerST(sys, sampler)
    return attacker

def assemble_sigma_attacker(sys: SystemEmulator, bf: BloomFilter) -> SigmaAttacker:
    attacker: SigmaAttacker = SigmaAttacker(sys, bf)
    return attacker

def get_attackers(
        attacker_configs: List[Dict[str, Any]], 
        systems:  List[SystemEmulator],
        training_set: StringArray,
        rng: RandomNumberGenerator,
    ) -> Tuple[List[str], List[Any]]:

    labels = []
    attackers = []

    for config, sys in zip(attacker_configs, systems):
        if (config["type"] == AttackerType.NAIVE):
            labels.append("Naive Attacker")
            attackers.append(assemble_naive_attacker(sys, rng, training_set))
        elif (config["type"] == AttackerType.TIMING):
            training_proportion = config["training_proportion"]
            labels.append(f"Timing Attacker Prop={training_proportion}")
            attackers.append(assemble_timing_attacker(sys, rng, training_set, training_proportion))
        elif (config["type"] == AttackerType.TIMING_ST):
            labels.append("Timing Attacker Single Training")
            attackers.append(assemble_timing_attacker_st(sys, rng, training_set))
        elif (config["type"] == AttackerType.SIGMA):
            labels.append("$\Sigma$igma Attacker")
            attackers.append(assemble_sigma_attacker(sys, sys.filter))

    return labels, attackers

def run_experiment_single_system(
        seed: int,
        dataset: StringArray,
        attacker_configs: List[Dict[str, Any]],
        initial_insert_size: int = 100000, 
        test_sample_size: int = 50000, 
        filter_buckets: int = 2**25,
        num_hash_functions: int = 5,
        system_delay: float = 1.0,
        attack_step_size: int = 500,
        attack_iterations: int = 50,
        output_dir: Path = None,
    ):

    insert_set = dataset[:initial_insert_size]
    training_set = dataset[:-test_sample_size]

    test_set = dataset[-test_sample_size:] # These are guaranteed to not be in the system, used for calculating false positive rate with known negatives

    rng: RandomNumberGenerator = RandomNumberGenerator(seed)

    systems = [assemble_system(filter_buckets, num_hash_functions, seed, system_delay, rng, insert_set) for _ in attacker_configs]

    attacker_labels, attackers = get_attackers(attacker_configs, systems, training_set, rng)

    fp_per_attacker = np.zeros((len(attackers), attack_iterations))

    for i, (attacker, system) in enumerate(zip(attackers, systems)):
        for j in tqdm(range(attack_iterations), desc=f"Attacker {attacker_labels[i]}"):
            fp_per_attacker[i,j] = run_pipeline(system, test_set, attacker, attack_step_size) / len(test_set)

    plt.figure()
    x = np.arange(attack_iterations) * attack_step_size
    for i in range(len(attackers)):
        plt.plot(x, fp_per_attacker[i], label=attacker_labels[i])

    plt.xlabel("Total Attack Size")
    plt.ylabel("False Positive Rate")
    plt.title(f"Bloom Filter Size: {filter_buckets}, Bloom Filter $k$: {num_hash_functions}, System Delay: {system_delay}")
    plt.legend()
    plt.tight_layout()
    if output_dir:
        plt.savefig(output_dir / f"filter_{filter_buckets}_k_{num_hash_functions}.png")
    plt.clf()

    # plot relative to last attacker (sigma)
    plt.figure()
    x = np.arange(attack_iterations) * attack_step_size
    for i in range(len(attackers)):
        plt.plot(x, fp_per_attacker[i] - fp_per_attacker[-1], label=attacker_labels[i])

    plt.xlabel("Total Attack Size")
    plt.ylabel("$\Delta$ False Positive Rate from $\Sigma$igma Attacker")
    plt.title(f"Bloom Filter Size: {filter_buckets}, Bloom Filter $k$: {num_hash_functions}, System Delay: {system_delay}")
    plt.legend()
    plt.tight_layout()
    if output_dir:
        plt.savefig(output_dir / f"filter_{filter_buckets}_k_{num_hash_functions}_relative.png")
    plt.clf()

def main():
    try:
        config = load_yaml("config.yaml")
    except Exception as e:
        print(e)
        sys.exit(1)


    ## *** REPLACE WITH YAML VALUES
    initial_insert_sizes = config.dataset.init_size # initial sample to insert into each system 
    test_sample_size = config.dataset.test_size # size of reserved testing set (for evaluating fp rate)

    bit_array_sizes = config.bloom_filter.bit_array_size

    num_hash_functions = config.bloom_filter.num_hash_functions

    system_delay = config.system_emulator.fixed_positive_delay_ms

    step_sizes = config.experiment.attack_size
    iterations = config.experiment.attack_iterations

    attacker_configs = []

    for attacker_type in config.attackers_enabled:
        if attacker_type == AttackerType.TIMING:
            for prop in config.attackers[attacker_type].training_proportion:
                attacker_configs.append({"type": attacker_type, "training_proportion": prop})
        else:
            attacker_configs.append({"type": attacker_type})

    dataset = StringArray(load_dataset(config.dataset.path))

    output_dir = Path(config.output_directory)
    output_dir.mkdir(exist_ok=True, parents=True)

    for bucket, init_size, step_size in zip(bit_array_sizes, initial_insert_sizes, step_sizes):
        run_experiment_single_system(
            config.seed,
            dataset,
            attacker_configs,
            init_size,
            test_sample_size,
            2**bucket,
            num_hash_functions,
            system_delay,
            step_size,
            iterations,
            output_dir
        )
    

if __name__ == "__main__":
    main()
