from c_modules import (
    SystemEmulator, 
    BloomFilter, 
    HashSet, 
    RandomNumberGenerator,
    StringArray,
    StringSampler,
    NaiveAttacker)

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
    return system.query_array(test_sample)

def assemble_system(buckets: int, num_hfs: int, seed: int, init_insert_size: int, urls) -> SystemEmulator:
    rng: RandomNumberGenerator = RandomNumberGenerator(seed)
    bf: BloomFilter = BloomFilter(buckets, num_hfs, rng)
    system: SystemEmulator = SystemEmulator(set(), bf, rng, 5.0)
    system.insert_array(urls[:init_insert_size])
    return system


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
