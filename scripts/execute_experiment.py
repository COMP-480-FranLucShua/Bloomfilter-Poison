from yaml_parser.loader import load_yaml
import pandas as pd
import sys

yaml_file_path = 'config.yaml'

def load_dataset(path: str):
    data = pd.read_csv(path, sep="\t")
    urllist = data.ClickURL.dropna().unique() # NOTE: 380k items
    return urllist

# ------------------------------------ UTILITIES ------------------------------------ 
def main():
    try:
        config = load_yaml("config.yaml")
    except Exception as e:
        print(e)
        sys.exit(1)

    load_dataset(config.dataset.path)


if __name__ == "__main__":
    main()
