from typing import Any
from .types import *

def parse_raw_to_config(raw: dict[str, Any]) -> ExperimentConfigRoot:
    return ExperimentConfigRoot(
        seed=raw["seed"],
        output_directory=raw["output_directory"],

        bloom_filter=BloomFilterConfig(**raw[Section.BLOOM_FILTER.value]),

        system_emulator=SystemEmulatorConfig(
            fixed_positive_delay_ms=raw[Section.SYSTEM_EMULATOR.value]["fixed_positive_delay_ms"]
        ),

        dataset=DatasetConfig(**raw[Section.DATASET.value]),

        experiment=ExperimentConfig(**raw[Section.EXPERIMENT.value]),

        attackers_enabled=[
            AttackerType(a) for a in raw[Section.ATTACKERS.value]["enabled"]
        ],

        attackers={
            AttackerType.NAIVE: NaiveAttackerConfig(
                **raw[Section.ATTACKERS.value][AttackerType.NAIVE.value]
            ),
            AttackerType.TIMING: TimingAttackerConfig(
                **raw[Section.ATTACKERS.value][AttackerType.TIMING.value]
            ),
            AttackerType.TIMING_ST: TimingAttackerSTConfig(
                **raw[Section.ATTACKERS.value][AttackerType.TIMING_ST.value]
            ),
            AttackerType.SIGMA: SigmaAttackerConfig(
            ),
        }
    )
