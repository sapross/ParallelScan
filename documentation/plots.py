def figure01(plotwrapper):
    columns = {
        "mp-media-gnu_inc_seq_sequential": "Naive (STL)",
        "mp-media-gnu_inc_seq_updown": "Up-Down Sweep",
        "mp-media-gnu_inc_seq_tiled": "Tiled Scan",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="Mp-Media Sequential Inclusive Scans",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


def figure02(plotwrapper):
    columns = {
        "mp-media-gnu_inc_seq_sequential": "Naive (STL)",
        "mp-media-gnu_inc_seq_updown": "Up-Down Sweep",
        "mp-media-gnu_inc_seq_tiled": "Tiled Scan",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="Mp-Media Sequential Exclusive Scans",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


def figure03(plotwrapper):
    columns = {
        "mp-media-gnu_inc_seq_sequential": "Naive (STL)",
        "mp-media-gnu_inc_seq_updown": "Up-Down Sweep",
        "mp-media-gnu_inc_seq_tiled": "Tiled Scan",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="Mp-Media Sequential Segmented Exclusive Scans",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


def figure04(plotwrapper):
    columns = {
        "mp-media-gnu_inc_seq_sequential": "Naive (STL)",
        "mp-media-gnu_inc_seq_updown": "Up-Down Sweep",
        "mp-media-gnu_inc_seq_tiled": "Tiled Scan",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="Mp-Media Sequential Segmented Inclusive Scans",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


def figure05(plotwrapper):
    columns = {
        "mp-media-gnu_inc_seq_sequential": "Naive (STL)",
        "mp-media-gnu_inc_OMP_provided": "OpenMP Provided Scan",
        "mp-media-gnu_inc_OMP_updown": "OpenMP Up-Down Sweep",
        "mp-media-gnu_inc_OMP_tiled": "OpenMP Tiled Scan",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="Mp-Media Parallel Inclusive Scans",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


def figure06(plotwrapper):
    columns = {
        "mp-media-gnu_ex_seq_sequential": "Naive (STL)",
        "mp-media-gnu_ex_OMP_provided": "OpenMP Provided Scan",
        "mp-media-gnu_ex_OMP_updown": "OpenMP Up-Down Sweep",
        "mp-media-gnu_ex_OMP_tiled": "OpenMP Tiled Scan",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="Mp-Media Parallel Exclusive Scans",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


def figure07(plotwrapper):
    columns = {
        "mp-media-gnu_inc_seq_sequential": "Naive (STL)",
        "mp-media-gnu_inc_seq_updown": "Up-Down Sweep",
        "mp-media-gnu_inc_seq_tiled": "Tiled Scan",
        "mp-media-gnu-algopt_inc_seq_sequential": "AlgOpt Naive (STL)",
        "mp-media-gnu-algopt_inc_seq_updown": "AlgOpt Up-Down Sweep",
        "mp-media-gnu-algopt_inc_seq_tiled": "AlgOpt Tiled Scan",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="Mp-Media Alg. Optimized Sequential Inclusive Scans",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


def figure08(plotwrapper):
    columns = {
        "mp-media-gnu_ex_seq_sequential": "Naive (STL)",
        "mp-media-gnu_ex_seq_updown": "Up-Down Sweep",
        "mp-media-gnu_ex_seq_tiled": "Tiled Scan",
        "mp-media-gnu-algopt_ex_seq_sequential": "AlgOpt Naive (STL)",
        "mp-media-gnu-algopt_ex_seq_updown": "AlgOpt Up-Down Sweep",
        "mp-media-gnu-algopt_ex_seq_tiled": "AlgOpt Tiled Scan",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="Mp-Media Alg. Optimized Sequential Exclusive Scans",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


def figure09(plotwrapper):
    columns = {
        "mp-media-gnu_incseg_seq_sequential": "Naive (STL)",
        "mp-media-gnu_incseg_seq_updown": "Up-Down Sweep",
        "mp-media-gnu_incseg_seq_tiled": "Tiled Scan",
        "mp-media-gnu-algopt_incseg_seq_sequential": "AlgOpt Naive (STL)",
        "mp-media-gnu-algopt_incseg_seq_updown": "AlgOpt Up-Down Sweep",
        "mp-media-gnu-algopt_incseg_seq_tiled": "AlgOpt Tiled Scan",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="Mp-Media Alg. Optimized Sequential Segmented Exclusive Scans",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


def figure10(plotwrapper):
    columns = {
        "mp-media-gnu_exseg_seq_sequential": "Naive (STL)",
        "mp-media-gnu_exseg_seq_updown": "Up-Down Sweep",
        "mp-media-gnu_exseg_seq_tiled": "Tiled Scan",
        "mp-media-gnu-algopt_exseg_seq_sequential": "AlgOpt Naive (STL)",
        "mp-media-gnu-algopt_exseg_seq_updown": "AlgOpt Up-Down Sweep",
        "mp-media-gnu-algopt_exseg_seq_tiled": "AlgOpt Tiled Scan",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="Mp-Media Alg. Optimized Sequential Segmented Inclusive Scans",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


def figure11(plotwrapper):
    columns = {
        "ziti-rome-gnu_inc_seq_sequential": "Naive (STL)",
        "ziti-rome-gnu_inc_seq_updown": "Up-Down Sweep",
        "ziti-rome-gnu_inc_seq_tiled": "Tiled Scan",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="Ziti-Rome Sequential Inclusive Scans",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


def figure12(plotwrapper):
    columns = {
        "ziti-rome-gnu_inc_seq_sequential": "Naive (STL)",
        "ziti-rome-gnu_inc_seq_updown": "Up-Down Sweep",
        "ziti-rome-gnu_inc_seq_tiled": "Tiled Scan",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="Ziti-Rome Sequential Exclusive Scans",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


def figure13(plotwrapper):
    columns = {
        "ziti-rome-gnu_inc_seq_sequential": "Naive (STL)",
        "ziti-rome-gnu_inc_seq_updown": "Up-Down Sweep",
        "ziti-rome-gnu_inc_seq_tiled": "Tiled Scan",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="Ziti-Rome Sequential Segmented Exclusive Scans",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


def figure14(plotwrapper):
    columns = {
        "ziti-rome-gnu_inc_seq_sequential": "Naive (STL)",
        "ziti-rome-gnu_inc_seq_updown": "Up-Down Sweep",
        "ziti-rome-gnu_inc_seq_tiled": "Tiled Scan",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="Ziti-Rome Sequential Segmented Inclusive Scans",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


def figure15(plotwrapper):
    columns = {
        "ziti-rome-gnu_inc_seq_sequential": "Naive (STL)",
        "ziti-rome-gnu_inc_OMP_provided": "OpenMP Provided Scan",
        "ziti-rome-gnu_inc_OMP_updown": "OpenMP Up-Down Sweep",
        "ziti-rome-gnu_inc_OMP_tiled": "OpenMP Tiled Scan",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="Ziti-Rome Parallel Inclusive Scans",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


def figure16(plotwrapper):
    columns = {
        "ziti-rome-gnu_ex_seq_sequential": "Naive (STL)",
        "ziti-rome-gnu_ex_OMP_provided": "OpenMP Provided Scan",
        "ziti-rome-gnu_ex_OMP_updown": "OpenMP Up-Down Sweep",
        "ziti-rome-gnu_ex_OMP_tiled": "OpenMP Tiled Scan",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="Ziti-Rome Parallel Exclusive Scans",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


"""
def figure07(plotwrapper):
    columns = {
        "mp-media-gnu_incseg_seq_sequential": "Naive (STL)",
        "mp-media-gnu_incseg_OMP_provided": "OpenMP Provided Scan",
        "mp-media-gnu_incseg_OMP_updown": "OpenMP Up-Down Sweep",
        "mp-media-gnu_incseg_OMP_tiled": "OpenMP Tiled Scan",
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
        "mp-media-gnu_exseg_seq_sequential": "Naive (STL)",
        "mp-media-gnu_exseg_OMP_provided": "OpenMP Provided Scan",
        "mp-media-gnu_exseg_OMP_updown": "OpenMP Up-Down Sweep",
        "mp-media-gnu_exseg_OMP_tiled": "OpenMP Tiled Scan",
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
