def figure01(plotwrapper):
    columns = [
        "home_Sequential Naive",
        "home_Sequential Up-Down",
        "home_Sequential Tiled",
    ]
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="Sequential Scans",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )
