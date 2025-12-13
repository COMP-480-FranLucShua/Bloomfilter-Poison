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
    initial_insert_size = 100000 # initial sample to 
    test_sample_size = 50000 # size of reserved testing set (for evaluating fp rate)

    dataset = load_dataset(config.dataset.path)

    rng: RandomNumberGenerator = RandomNumberGenerator(seed)


    with RandomNumberGenerator(12345):
        set = HashSet(256, 123456)
        rng = RandomNumberGenerator(123456)
        filter = BloomFilter(4, 2, rng)

        system = SystemEmulator(set, filter, rng, 1.0)

        data = StringArray(["abc", "def", "ge", "abce", "efg", "hijk"])

        sampler = StringSampler(data, rng)

        attacker = NaiveAttacker(system, sampler)

        attacker.attack(4)

        sample = sampler.sample_array(len(data))

        print(system.query_array(sample))

        attacker.close()
        sampler.close()
        system.close()
        filter.close()
        rng.close()
        set.close()

if __name__ == "__main__":
    main()
