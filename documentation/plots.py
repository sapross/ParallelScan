def figure01(plotwrapper):
    columns = {
        "mp-media_inc_seq_sequential": "Naive (STL)",
        "mp-media_inc_seq_updown": "Up-Down Sweep",
        "mp-media_inc_seq_tiled": "Tiled Scan",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="Sequential Inclusive Scans",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


def figure02(plotwrapper):
    columns = {
        "mp-media_inc_seq_sequential": "Naive (STL)",
        "mp-media_inc_seq_updown": "Up-Down Sweep",
        "mp-media_inc_seq_tiled": "Tiled Scan",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="Sequential Exclusive Scans",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


def figure03(plotwrapper):
    columns = {
        "mp-media_inc_seq_sequential": "Naive (STL)",
        "mp-media_inc_seq_updown": "Up-Down Sweep",
        "mp-media_inc_seq_tiled": "Tiled Scan",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="Sequential Segmented Exclusive Scans",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


def figure04(plotwrapper):
    columns = {
        "mp-media_inc_seq_sequential": "Naive (STL)",
        "mp-media_inc_seq_updown": "Up-Down Sweep",
        "mp-media_inc_seq_tiled": "Tiled Scan",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="Sequential Segmented Inclusive Scans",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


def figure05(plotwrapper):
    columns = {
        "mp-media_inc_seq_sequential": "Naive (STL)",
        "mp-media_inc_OMP_provided": "OpenMP Provided Scan",
        "mp-media_inc_OMP_updown": "OpenMP Up-Down Sweep",
        "mp-media_inc_OMP_tiled": "OpenMP Tiled Scan",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="Parallel Inclusive Scans",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


def figure06(plotwrapper):
    columns = {
        "mp-media_ex_seq_sequential": "Naive (STL)",
        "mp-media_ex_OMP_provided": "OpenMP Provided Scan",
        "mp-media_ex_OMP_updown": "OpenMP Up-Down Sweep",
        "mp-media_ex_OMP_tiled": "OpenMP Tiled Scan",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="Parallel Exclusive Scans",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


"""
def figure07(plotwrapper):
    columns = {
        "mp-media_incseg_seq_sequential": "Naive (STL)",
        "mp-media_incseg_OMP_provided": "OpenMP Provided Scan",
        "mp-media_incseg_OMP_updown": "OpenMP Up-Down Sweep",
        "mp-media_incseg_OMP_tiled": "OpenMP Tiled Scan",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="Parallel Inclusive Segmented Scans",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


def figure08(plotwrapper):
    columns = {
        "mp-media_exseg_seq_sequential": "Naive (STL)",
        "mp-media_exseg_OMP_provided": "OpenMP Provided Scan",
        "mp-media_exseg_OMP_updown": "OpenMP Up-Down Sweep",
        "mp-media_exseg_OMP_tiled": "OpenMP Tiled Scan",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="Parallel Exclusive Segmented Scans",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )
"""
