import pandas as pd
import matplotlib as mp
from pathlib import Path


class ResultAgglomerator:
    # csv separator
    separator = "\t"
    header = {0: "Name", 1: "Throughput", 2: "N"}
    # Datatype used in the benchmarks, in our case float = 4 Bytes
    operand_size = 4.0

    def __init__(self):
        self.dataframe = pd.DataFrame()

    def add_data(self, data_path=Path()):
        new_df = pd.read_csv(
            str(data_path), sep=self.separator, header=None
        )
        for i in range(2, len(new_df.columns)):
            # label = df[i][0].split(" ")[0]
            # headers[i] = label
            # Extract base10 value from string. Example:
            # N := 16777216 (0x1000000)
            #      ^^^^^^^^
            new_df[i] = new_df[i].str.split(" ").str.get(2)

        new_df[1] = (
            new_df[2].astype(float) * 4.0 / new_df[1].astype(float)
        )
        new_df[0] = new_df[0].apply(
            lambda value: data_path.name.split("_")[0] + "_" + value
        )
        new_df = new_df.rename(columns=self.header)

        if self.dataframe.empty:
            self.dataframe = new_df
        else:
            self.dataframe = pd.concat(
                [self.dataframe, new_df], axis=0
            )

        print(self.dataframe)

    def get_results(self, benchmarks=[]):
        df = self.dataframe.pivot(
            columns="Name", values="Throughput", index="N"
        ).sort_values(by=["N"], key=lambda col: col.astype(int))
        print(df[benchmarks])
        return df[benchmarks]


if __name__ == "__main__":
    agglomerator = ResultAgglomerator()
    agglomerator.add_data(Path("../home_results.csv"))
    agglomerator.add_data(Path("../home01_results.csv"))
    agglomerator.get_results(
        ["home_Sequential Naive", "home01_Sequential Naive"]
    )
