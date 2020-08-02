import argparse

import pandas as pd

parser = argparse.ArgumentParser(description="Accepts filename")
parser.add_argument('--filename', '-f', type=str, required=True, help='Path to file to arrange')
args = parser.parse_args()

filename = args.filename
print(filename)

df = pd.read_csv(filename)
cols = list(df.columns.values)
df = df[ [ cols[0], cols[3], cols[4], cols[5], cols[1], cols[2] ] ]

df.to_csv('testing', index=False)
