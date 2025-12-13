import yaml
from typing import Any
from .types import *
from .parser import parse_raw_to_config

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
