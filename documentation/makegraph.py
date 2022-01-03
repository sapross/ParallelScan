"""Scriptable plotter for automatic collection of results from multiple csv.
Usage:

> makeplot file 'home.csv' col 'home_seq_naive' plot --title 'Naive Sequential'
Reads home.csv and plots the data from column titled 'home_seq_naive'.
Multiple columns may be selected by chaining additional col 'name' commands.

> makeplot filedir 'results' plotall
Read all .csv files contained in the results directory and create all plots
defined in plots.py.

Plot will be saved in graphs/Naive\ Sequential.pdf.

"""
from pathlib import Path
import pandas as pd
import matplotlib.pyplot as plt
import fire


class ResultAggregator:
    """
    Class for collecting results from our catch2 benchmarks and
    outputting easily plottable tables.
    """

    # csv separator
    separator = "\t"
    header = {0: "Name", 1: "Throughput", 2: "N"}
    # Datatype used in the benchmarks, in our case float = 4 Bytes
    operand_size = 4.0

    def __init__(self):
        self.dataframe = pd.DataFrame()

    def __str__(self):
        return str(self.dataframe)

    def add_data(self, data_path=Path()):
        """Reads csv file and concatenates the result to the internal
        dataframe.
        Warning: The filename is inserted before the benchmark names to
        allow easy aggregation of the same benchmarks from multiple
        machines.
        Example: mp-media.csv
            seq_naive -> mp-media_seq_naive

            Parameters:
               data_path (Path): Path object pointing to csv file.

            Returns:
               None
        """
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

        # Insert filename into later column names.
        new_df[0] = new_df[0].apply(
            lambda value: data_path.name.split(".")[0] + "_" + value
        )

        new_df = new_df.rename(columns=self.header)

        if self.dataframe.empty:
            self.dataframe = new_df
        else:
            self.dataframe = pd.concat(
                [self.dataframe, new_df], axis=0
            )

    def get_results(self, benchnames=[]):
        """Pivots dataframe and returns the selected columns.

        Parameters:
           benchnames (list(str)): Benchmark names of the desired
                                benchmark results.
        Returns:
           Pandas.DataFrame: Benchmark results with index of 'N' and
                             selected results column-wise.
        """
        df = self.dataframe.pivot(
            columns="Name", values="Throughput", index="N"
        ).sort_values(by=["N"], key=lambda col: col.astype(int))
        # Plotting logscale with index is bugged
        # N must be added again
        df["N"] = df.index.astype(int)
        print(["N"] + benchnames)

        return df[["N"] + benchnames]


class Plotwrapper:
    def __init__(self):
        self.agg = ResultAggregator()
        self.columns = []

    def col(self, column: str):
        self.columns.append(column)
        return self

    def plot(
        self,
        title: str = "Plot",
        index_col: str = "N",
        xscale: str = "log",
        yscale: str = "linear",
        grid: bool = True,
    ):
        """Plots data from ResultAggregator with benchmarks selected from
        columns. Figures are saved as .pdf in './graphs/'
        """
        data = self.agg.get_results(self.columns)
        data.plot(x=index_col, marker="x")
        plt.title(title)
        plt.xlabel(index_col)
        plt.ylabel("GB/s")
        if xscale == "log":
            plt.xscale("log", base=2)
        else:
            plt.xscale(xscale)
        plt.yscale(yscale)

        if grid:
            plt.grid()

        plt.savefig("graphs/" + title + ".pdf")

    def file(self, csv: str):
        self.agg.add_data(Path(csv))
        return self

    def filedir(self, dirpath: str):
        for fpath in Path().glob(dirpath + "/*.csv"):
            self.agg.add_data(fpath)
        return self

    def plotall(self):
        """Plot all plots defined as functions in plots.py"""
        import plots as p

        for i in dir(p):
            plot = getattr(p, i)
            if callable(plot):
                plot(self)


if __name__ == "__main__":
    fire.Fire(Plotwrapper)
