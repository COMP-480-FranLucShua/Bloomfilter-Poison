import numpy as np
import matplotlib.pyplot as plt

from c_modules import (
    SystemEmulator, 
    BloomFilter, 
    HashSet, 
    RandomNumberGenerator,
    StringArray,
    StringSampler,
    NaiveAttacker,
    DataArray,
    TimingAttacker,
    SigmaAttacker,
)

from yaml_parser.loader import load_yaml
import pandas as pd
import sys

yaml_file_path = 'config.yaml'

# ------------------------------------ UTILITIES ------------------------------------ 

def load_dataset(path: str):
    data = pd.read_csv(path, sep="\t")
    urllist = data.ClickURL.dropna().unique() # NOTE: 380k items
    return urllist

def run_pipeline(system, test_sample, attacker, attack_size):
    # print("Beginning attack")
    # print(attacker.sampler._strings[:10].to_python())
    attacker.attack(attack_size)
    return system.query_array(test_sample)

def assemble_system(buckets: int, num_hfs: int, seed: int, rng: RandomNumberGenerator, initial_insert: StringArray) -> SystemEmulator:
    bf: BloomFilter = BloomFilter(buckets, num_hfs, rng)
    system: SystemEmulator = SystemEmulator(HashSet(1024, seed), bf, rng, 5.0)
    system.insert_array(initial_insert)
    return system

def assemble_naive_attacker(sys: SystemEmulator, rng: RandomNumberGenerator, dataset: StringArray) -> NaiveAttacker:
    sampler: StringSampler = StringSampler(dataset, rng)
    attacker: NaiveAttacker = NaiveAttacker(sys, sampler)
    return attacker

def assemble_timing_attacker(sys: SystemEmulator, rng: RandomNumberGenerator, dataset: StringArray) -> TimingAttacker:
    sampler: StringSampler = StringSampler(dataset, rng)
    attacker: TimingAttacker = TimingAttacker(sys, sampler, 0.15)
    return attacker

def assemble_sigma_attacker(sys: SystemEmulator, bf: BloomFilter) -> SigmaAttacker:
    attacker: SigmaAttacker = SigmaAttacker(sys, bf)
    return attacker

def main():
    try:
        config = load_yaml("config.yaml")
    except Exception as e:
        print(e)
        sys.exit(1)

    print("Starting")

    ## *** REPLACE WITH YAML VALUES
    initial_insert_size = 10 # initial sample to 
    # test_sample_size = 50000 # size of reserved testing set (for evaluating fp rate)

    dataset = StringArray(load_dataset(config.dataset.path))

    insert_set = dataset[:initial_insert_size]
    training_set = dataset[:-10000]
    test_set = dataset[-10000:]

    print("Created datasets")

    print(config.seed)

    rng: RandomNumberGenerator = RandomNumberGenerator(config.seed)
    system_naive = assemble_system(1024, 3, config.seed, rng, insert_set)
    system_timing = assemble_system(1024, 3, config.seed, rng, insert_set)
    system_sigma = assemble_system(1024, 3, config.seed, rng, insert_set)

    print("Created systems")


    attacker_naive = assemble_naive_attacker(system_naive, rng, training_set)
    attacker_timing = assemble_timing_attacker(system_timing, rng, training_set)
    attacker_sigma = assemble_sigma_attacker(system_sigma, system_sigma.filter)
    
    step_size = 100
    iterations = 20

    fp_per_attacker = np.zeros((3, iterations))

    print("Initialized attack objects")


    labels = ["Naive", "Timing", "Sigma"]

    for i, (attacker, system) in enumerate([(attacker_naive, system_naive), (attacker_timing, system_timing), (attacker_sigma, system_sigma)]):
        print(f"Running attack: {labels[i]}")
        for j in range(iterations):
            fp_per_attacker[i,j] = run_pipeline(system, test_set, attacker, step_size)


    plt.figure()
    x = np.arange(iterations)
    for i in range(3):
        plt.plot(x, fp_per_attacker[i], label=labels[i])

    plt.xlabel("Iteration")
    plt.ylabel("False Positive Rate")
    plt.title("False Positive Rate per Attacker")
    plt.legend()
    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    main()
