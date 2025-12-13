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
    attacker.attack(attack_size)
    return system.query_array(DataArray.from_array(test_sample))

def assemble_system(buckets: int, num_hfs: int, seed: int, init_insert_size: int, rng: RandomNumberGenerator, urls) -> SystemEmulator:
    bf: BloomFilter = BloomFilter(buckets, num_hfs, rng)
    system: SystemEmulator = SystemEmulator(HashSet(1024, seed), bf, rng, 5.0)
    system.insert_array(DataArray.from_array(urls[:init_insert_size]))
    return system

def assemble_naive_attacker(sys: SystemEmulator, rng: RandomNumberGenerator, urls) -> NaiveAttacker:
    sampler: StringSampler = StringSampler(StringArray(urls), rng)
    attacker: NaiveAttacker = NaiveAttacker(sys, sampler)
    return attacker

def assemble_timing_attacker(sys: SystemEmulator, rng: RandomNumberGenerator, urls) -> TimingAttacker:
    sampler: StringSampler = StringSampler(StringArray(urls), rng)
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

    ## *** REPLACE WITH YAML VALUES
    # initial_insert_size = 100000 # initial sample to 
    # test_sample_size = 50000 # size of reserved testing set (for evaluating fp rate)

    dataset = load_dataset(config.dataset.path)

    training_set = dataset[:-10000]
    test_set = dataset[-10000:]

    rng: RandomNumberGenerator = RandomNumberGenerator(config.seed)
    system_naive = assemble_system(1024, 3, config.seed, 10, rng, training_set)
    system_timing = assemble_system(1024, 3, config.seed, 10, rng, training_set)
    system_sigma = assemble_system(1024, 3, config.seed, 10, rng, training_set)

    attacker_naive = assemble_naive_attacker(system_naive, rng, training_set)
    attacker_timing = assemble_timing_attacker(system_timing, rng, training_set)
    attacker_sigma = assemble_sigma_attacker(system_sigma, system_sigma.filter)
    
    step_size = 100
    iterations = 20

    test_sample = DataArray.from_array(test_set)

    fp_per_attacker = np.zeros((3, iterations))

    for i, (attacker, system) in enumerate([(attacker_naive, system_naive), (attacker_timing, system_timing), (attacker_sigma, system_sigma)]):
        for j in range(iterations):
            fp_per_attacker[i,j] = run_pipeline(system, test_sample, attacker, step_size)

    labels = ["Naive", "Timing", "Sigma"]

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
