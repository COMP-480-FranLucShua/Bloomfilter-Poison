from enum import Enum
from dataclasses import dataclass

class Section(str, Enum):
    BLOOM_FILTER = "bloom_filter"
    SYSTEM_EMULATOR = "system_emulator"
    DATASET = "dataset"
    EXPERIMENT = "experiment"
    ATTACKERS = "attackers"
    LOGGING = "logging"
    METRICS = "metrics"


class AttackerType(str, Enum):
    NAIVE = "naive"
    TIMING = "timing"
    SIGMA = "sigma"


@dataclass(frozen=True)
class BloomFilterConfig:
    bit_array_size: int
    num_hash_functions: int
    target_false_positive_rate: float


@dataclass(frozen=True)
class GammaDistributionConfig:
    shape: float
    scale: float


@dataclass(frozen=True)
class SystemEmulatorConfig:
    gamma_distribution: GammaDistributionConfig
    fixed_positive_delay_ms: float


@dataclass(frozen=True)
class DatasetConfig:
    path: str
    benign_label: int
    malicious_label: int
    preload_malicious_count: int
    attacker_url_pool_size: int
    evaluation_set_size: int


@dataclass(frozen=True)
class ExperimentConfig:
    max_insertions_per_attacker: int
    measurement_interval: int
    num_trials: int


@dataclass(frozen=True)
class NaiveAttackerConfig:
    batch_size: int


@dataclass(frozen=True)
class TimingAttackerConfig:
    initial_sample_size: int
    kmeans_clusters: int
    max_retries_per_step: int


@dataclass(frozen=True)
class SigmaAttackerConfig:
    pass


@dataclass(frozen=True)
class ExperimentConfigRoot:
    seed: int
    bloom_filter: BloomFilterConfig
    system_emulator: SystemEmulatorConfig
    dataset: DatasetConfig
    experiment: ExperimentConfig
    attackers_enabled: list[AttackerType]
    attackers: dict[AttackerType, object]


# ------------------------------------ UTILITIES ------------------------------------ 
def load_yaml(yaml_file_path: str) -> ExperimentConfigRoot:
    try:
        with open(yaml_file_path, "r") as file:
            raw: dict[str, Any] = yaml.safe_load(file)

        return parse_raw_to_config(raw)

    except FileNotFoundError as e:
        raise FileNotFoundError(
            f"Config file '{yaml_file_path}' not found"
        ) from e

    except yaml.YAMLError as e:
        raise ValueError(
            f"Invalid YAML syntax in '{yaml_file_path}'"
        ) from e

    except KeyError as e:
        raise ValueError(
            f"Missing required configuration field: {e}"
        ) from e

    except TypeError as e:
        raise ValueError(
            f"Invalid configuration schema: {e}"
        ) from e

