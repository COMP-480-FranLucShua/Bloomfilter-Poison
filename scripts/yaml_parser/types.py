from enum import Enum
from dataclasses import dataclass

class Section(str, Enum):
    BLOOM_FILTER = "bloom_filter"
    SYSTEM_EMULATOR = "system_emulator"
    DATASET = "dataset"
    EXPERIMENT = "experiment"
    ATTACKERS = "attackers"
    LOGGING = "logging"


class AttackerType(str, Enum):
    NAIVE = "naive"
    TIMING = "timing"
    TIMING_ST = "timing_st"
    SIGMA = "sigma"

@dataclass(frozen=True)
class BloomFilterConfig:
    bit_array_size: list[int]
    num_hash_functions: int


@dataclass(frozen=True)
class SystemEmulatorConfig:
    fixed_positive_delay_ms: float


@dataclass(frozen=True)
class DatasetConfig:
    path: str
    init_size: list[int]
    test_size: int

@dataclass(frozen=True)
class NaiveAttackerConfig:
    pass

@dataclass(frozen=True)
class TimingAttackerConfig:
    training_proportion: list[float]

@dataclass(frozen=True)
class TimingAttackerSTConfig:
    pass

@dataclass(frozen=True)
class SigmaAttackerConfig:
    pass

@dataclass(frozen=True)
class ExperimentConfig:
    attack_size: list[int]
    attack_iterations: int

@dataclass(frozen=True)
class ExperimentConfigRoot:
    seed: int
    bloom_filter: BloomFilterConfig
    system_emulator: SystemEmulatorConfig
    dataset: DatasetConfig
    experiment: ExperimentConfig
    attackers_enabled: list[AttackerType]
    attackers: dict[AttackerType, object]
    output_directory: str
