systems = ["mp-media", "ziti-rome"]

# ---------------------------------------------------------------------------------
# Sequential Plots
# ---------------------------------------------------------------------------------
def figure_sequential(plotwrapper):
    columns = {
        "gnu_inc_seq_sequential": "STL Inc",
        "gnu_ex_seq_sequential": "STL Ex",
        "gnu_inc_seq_updown": "Up-Down Inc",
        "gnu_ex_seq_updown": "Up-Down Ex",
        "gnu_inc_seq_tiled": "Tiled Inc",
        "gnu_ex_seq_tiled": "Tiled Ex",
    }
    for system in systems:
        syscol = dict()

        for key, value in columns.items():
            syscol[system + "-" + key] = value

        plotwrapper.columns = syscol
        plotwrapper.plot(
            title=system + " Sequential Scans",
            index_col="N",
            xscale="log",
            yscale="linear",
            grid=True,
        )


def figure_sequential_segmented(plotwrapper):
    columns = {
        "gnu_incseg_seq_sequential": "STL Inc",
        "gnu_exseg_seq_sequential": "STL Ex",
        "gnu_incseg_seq_updown": "Up-Down Inc",
        "gnu_exseg_seq_updown": "Up-Down Ex",
        "gnu_incseg_seq_tiled": "Tiled Inc",
        "gnu_exseg_seq_tiled": "Tiled Ex",
    }
    for system in systems:
        syscol = dict()

        for key, value in columns.items():
            syscol[system + "-" + key] = value

        plotwrapper.columns = syscol
        plotwrapper.plot(
            title=system + " Sequential Segmented Scans",
            index_col="N",
            xscale="log",
            yscale="linear",
            grid=True,
        )


# ---------------------------------------------------------------------------------
# Parallel Plots
# ---------------------------------------------------------------------------------


def figure_parallel_media(plotwrapper):
    columns = {
        "gnu_inc_seq_sequential": "STL",
        "gnu_inc_OMP_provided": "OpenMP Provided Scan",
        "gnu_inc_TBB_provided": "TBB Provided Scan",
        "gnu_inc_OMP_updown": "OpenMP Up-Down Sweep",
        "gnu_inc_TBB_updown": "TBB Up-Down Sweep",
        "gnu_inc_OMP_tiled": "OpenMP Tiled Scan",
        "gnu_inc_TBB_tiled": "TBB Tiled Scan",
    }
    for system in systems:
        syscol = dict()

        for key, value in columns.items():
            syscol[system + "-" + key] = value

        plotwrapper.columns = syscol
        plotwrapper.plot(
            title=system + " Parallel Inclusive Scans",
            index_col="N",
            xscale="log",
            yscale="linear",
            grid=True,
        )


def figure_parallel_segmented(plotwrapper):
    columns = {
        "gnu_incseg_seq_sequential": "STL",
        "gnu_incseg_TBB_provided": "TBB Provided Scan",
        "gnu_incseg_OMP_updown": "OpenMP Up-Down Sweep",
        "gnu_incseg_TBB_updown": "TBB Up-Down Sweep",
        "gnu_incseg_OMP_tiled": "OpenMP Tiled Scan",
        "gnu_incseg_TBB_tiled": "TBB Tiled Scan",
    }
    for system in systems:
        syscol = dict()

        for key, value in columns.items():
            syscol[system + "-" + key] = value

        plotwrapper.columns = syscol
        plotwrapper.plot(
            title=system + " Parallel Inclusive Segmented Scans",
            index_col="N",
            xscale="log",
            yscale="linear",
            grid=True,
        )


# ---------------------------------------------------------------------------------
# Algorithmic Optimization Plots
# ---------------------------------------------------------------------------------


def figure_algopt_inclusive(plotwrapper):
    columns = {
        "gnu_inc_seq_sequential": "STL",
        "gnu-algopt_inc_seq_sequential": "AlgOpt STL",
        "gnu_inc_seq_updown": "Up-Down Sweep",
        "gnu-algopt_inc_seq_updown": "AlgOpt Up-Down Sweep",
        "gnu_inc_seq_tiled": "Tiled Scan",
        "gnu-algopt_inc_seq_tiled": "AlgOpt Tiled Scan",
    }
    for system in systems:
        syscol = dict()

        for key, value in columns.items():
            syscol[system + "-" + key] = value

        plotwrapper.columns = syscol
        plotwrapper.plot(
            title=system
            + " Alg. Optimized Sequential Inclusive Scans",
            index_col="N",
            xscale="log",
            yscale="linear",
            grid=True,
        )


def figure_algopt_exclusive_media(plotwrapper):
    columns = {
        "gnu_ex_seq_sequential": "Naive (STL)",
        "gnu-algopt_ex_seq_sequential": "AlgOpt Naive (STL)",
        "gnu_ex_seq_updown": "Up-Down Sweep",
        "gnu-algopt_ex_seq_updown": "AlgOpt Up-Down Sweep",
        "gnu_ex_seq_tiled": "Tiled Scan",
        "gnu-algopt_ex_seq_tiled": "AlgOpt Tiled Scan",
    }
    for system in systems:
        syscol = dict()

        for key, value in columns.items():
            syscol[system + "-" + key] = value

        plotwrapper.columns = syscol
        plotwrapper.plot(
            title=system
            + " Alg. Optimized Sequential Exclusive Scans",
            index_col="N",
            xscale="log",
            yscale="linear",
            grid=True,
        )


def figure_algopt_inclusive_segmented(plotwrapper):
    columns = {
        "gnu_incseg_seq_sequential": "Naive (STL)",
        "gnu_incseg_seq_updown": "Up-Down Sweep",
        "gnu_incseg_seq_tiled": "Tiled Scan",
        "gnu-algopt_incseg_seq_sequential": "AlgOpt Naive (STL)",
        "gnu-algopt_incseg_seq_updown": "AlgOpt Up-Down Sweep",
        "gnu-algopt_incseg_seq_tiled": "AlgOpt Tiled Scan",
    }
    for system in systems:
        syscol = dict()

        for key, value in columns.items():
            syscol[system + "-" + key] = value

        plotwrapper.columns = syscol
        plotwrapper.plot(
            title=system
            + " Alg. Optimized Sequential Segmented Inclusive Scans",
            index_col="N",
            xscale="log",
            yscale="linear",
            grid=True,
        )


def figure_algopt_exclusive_segmented(plotwrapper):
    columns = {
        "gnu_exseg_seq_sequential": "Naive (STL)",
        "gnu_exseg_seq_updown": "Up-Down Sweep",
        "gnu_exseg_seq_tiled": "Tiled Scan",
        "gnu-algopt_exseg_seq_sequential": "AlgOpt Naive (STL)",
        "gnu-algopt_exseg_seq_updown": "AlgOpt Up-Down Sweep",
        "gnu-algopt_exseg_seq_tiled": "AlgOpt Tiled Scan",
    }
    for system in systems:
        syscol = dict()

        for key, value in columns.items():
            syscol[system + "-" + key] = value

        plotwrapper.columns = syscol
        plotwrapper.plot(
            title=system
            + " Alg. Optimized Sequential Segmented Exclusive Scans",
            index_col="N",
            xscale="log",
            yscale="linear",
            grid=True,
        )


# ---------------------------------------------------------------------------------
# Tile Size Plots
# ---------------------------------------------------------------------------------


def figure_tilesize(plotwrapper):
    columns = {
        "gnu-tilesize_omp_inplace_inc_tilesize": "Inclusive",
        "gnu-tilesize_omp_inplace_ex_tilesize": "Exclusive",
        "gnu-tilesize_omp_inplace_incseg_tilesize": "Inclusive Segmented",
        "gnu-tilesize_omp_inplace_exseg_tilesize": "Exclusive Segmented",
    }
    for system in systems:
        syscol = dict()

        for key, value in columns.items():
            syscol[system + "-" + key] = value

        plotwrapper.columns = syscol
        plotwrapper.plot(
            title=system + " OpenMP Tile Size Benchmark",
            index_col="tile_size",
            xscale="log",
            yscale="linear",
            grid=True,
        )


# ---------------------------------------------------------------------------------
# OpenMP Scheduling Plots with SIMD
# ---------------------------------------------------------------------------------


def figure_provided_scheduling_SIMD_media(plotwrapper):
    columns = {
        "mp_media_gnu_omp_scheduling_static_media2_inc_OMP_provided": "Static",
        "mp_media_gnu_omp_scheduling_dynamic_media2_inc_OMP_provided": "Dynamic",
        "mp_media_gnu_omp_scheduling_guided_media2_inc_OMP_provided": "Guided",
        "mp_media_gnu_omp_scheduling_auto_media2_inc_OMP_provided": "Auto",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="mp_media OpenMP Provided Inclusive Scans \n with Different Scheduling and SIMD (gnu)",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


def figure_updown_scheduling_SIMD_media(plotwrapper):
    columns = {
        "mp_media_gnu_omp_scheduling_static_media2_inc_OMP_updown": "Static",
        "mp_media_gnu_omp_scheduling_dynamic_media2_inc_OMP_updown": "Dynamic",
        "mp_media_gnu_omp_scheduling_guided_media2_inc_OMP_updown": "Guided",
        "mp_media_gnu_omp_scheduling_auto_media2_inc_OMP_updown": "Auto",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="mp-media OpenMP Updown Inclusive Scans \n with Different Scheduling and SIMD (gnu)",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


def figure_tiled_scheduling_SIMD_media(plotwrapper):
    columns = {
        "mp_media_gnu_omp_scheduling_static_media2_inc_OMP_tiled": "Static",
        "mp_media_gnu_omp_scheduling_dynamic_media2_inc_OMP_tiled": "Dynamic",
        "mp_media_gnu_omp_scheduling_guided_media2_inc_OMP_tiled": "Guided",
        "mp_media_gnu_omp_scheduling_auto_media2_inc_OMP_tiled": "Auto",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="mp-media OpenMP Tiled Inclusive Scans \n with Different Scheduling and SIMD (gnu)",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


def figure_provided_scheduling_SIMD_rome(plotwrapper):
    columns = {
        "ziti_rome_gnu_omp_scheduling_static_inc_OMP_provided": "Static",
        "ziti_rome_gnu_omp_scheduling_dynamic_inc_OMP_provided": "Dynamic",
        "ziti_rome_gnu_omp_scheduling_guided_inc_OMP_provided": "Guided",
        "ziti_rome_gnu_omp_scheduling_auto_inc_OMP_provided": "Auto",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="ziti-rome OpenMP Provided Inclusive Scan with Different Scheduling and SIMD (gnu)",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


def figure_updown_scheduling_SIMD_rome(plotwrapper):
    columns = {
        "ziti_rome_gnu_omp_scheduling_static_inc_OMP_updown": "Static",
        "ziti_rome_gnu_omp_scheduling_dynamic_inc_OMP_updown": "Dynamic",
        "ziti_rome_gnu_omp_scheduling_guided_inc_OMP_updown": "Guided",
        "ziti_rome_gnu_omp_scheduling_auto_inc_OMP_updown": "Auto",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="ziti-rome OpenMP Updown Inclusive Scan with Different Scheduling and SIMD (gnu)",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


def figure_tiled_scheduling_SIMD_rome(plotwrapper):
    columns = {
        "ziti_rome_gnu_omp_scheduling_static_inc_OMP_tiled": "Static",
        "ziti_rome_gnu_omp_scheduling_dynamic_inc_OMP_tiled": "Dynamic",
        "ziti_rome_gnu_omp_scheduling_guided_inc_OMP_tiled": "Guided",
        "ziti_rome_gnu_omp_scheduling_auto_inc_OMP_tiled": "Auto",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="ziti-rome OpenMP Tiled Inclusive Scan with Different Scheduling and SIMD (gnu)",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


# ---------------------------------------------------------------------------------
# OpenMP SIMD vs no SIMD Plots
# ---------------------------------------------------------------------------------


def figure_SIMD_rome(plotwrapper):
    columns = {
        "ziti_rome_gnu_omp_scheduling_static_inc_OMP_tiled": "SIMD",
        "ziti_rome_gnu_omp_no_simd_scheduling_static_inc_OMP_tiled": "No SIMD",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="ziti-rome OpenMP Tiled Inclusive Scan (gnu)",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


def figure_SIMD_media(plotwrapper):
    columns = {
        "mp_media_gnu_omp_scheduling_static_media2_inc_OMP_tiled": "SIMD",
        "mp_media_gnu_omp_no_simd_scheduling_static_media2_inc_OMP_tiled": "No SIMD",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="mp-media OpenMP Tiled Inclusive Scan (gnu)",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


# ---------------------------------------------------------------------------------
# OpenMP Scheduling no SIMD Plots
# ---------------------------------------------------------------------------------


def figure_provided_scheduling_media(plotwrapper):
    columns = {
        "mp_media_gnu_omp_no_simd_scheduling_static_media2_inc_OMP_provided": "Static",
        "mp_media_gnu_omp_no_simd_scheduling_dynamic_media2_inc_OMP_provided": "Dynamic",
        "mp_media_gnu_omp_no_simd_scheduling_guided_media2_inc_OMP_provided": "Guided",
        "mp_media_gnu_omp_no_simd_scheduling_auto_media2_inc_OMP_provided": "Auto",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="mp-media OpenMP Provided Inclusive Scan with Different Scheduling (gnu)",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


def figure_updown_scheduling_media(plotwrapper):
    columns = {
        "mp_media_gnu_omp_no_simd_scheduling_static_media2_inc_OMP_updown": "Static",
        "mp_media_gnu_omp_no_simd_scheduling_dynamic_media2_inc_OMP_updown": "Dynamic",
        "mp_media_gnu_omp_no_simd_scheduling_guided_media2_inc_OMP_updown": "Guided",
        "mp_media_gnu_omp_no_simd_scheduling_auto_media2_inc_OMP_updown": "Auto",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="mp-media OpenMP Updown Inclusive Scans with Different Scheduling (gnu)",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


def figure_tiled_scheduling_media(plotwrapper):
    columns = {
        "mp_media_gnu_omp_no_simd_scheduling_static_media2_inc_OMP_tiled": "Static",
        "mp_media_gnu_omp_no_simd_scheduling_dynamic_media2_inc_OMP_tiled": "Dynamic",
        "mp_media_gnu_omp_no_simd_scheduling_guided_media2_inc_OMP_tiled": "Guided",
        "mp_media_gnu_omp_no_simd_scheduling_auto_media2_inc_OMP_tiled": "Auto",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="mp-media OpenMP Tiled Inclusive Scans with Different Scheduling (gnu)",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


def figure_provided_scheduling_rome(plotwrapper):
    columns = {
        "ziti_rome_gnu_omp_no_simd_scheduling_static_inc_OMP_provided": "Static",
        "ziti_rome_gnu_omp_no_simd_scheduling_dynamic_inc_OMP_provided": "Dynamic",
        "ziti_rome_gnu_omp_no_simd_scheduling_guided_inc_OMP_provided": "Guided",
        "ziti_rome_gnu_omp_no_simd_scheduling_auto_inc_OMP_provided": "Auto",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="ziti-rome OpenMP Provided Inclusive Scan with Different Scheduling (gnu)",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


def figure_updown_scheduling_rome(plotwrapper):
    columns = {
        "ziti_rome_gnu_omp_no_simd_scheduling_static_inc_OMP_updown": "Static",
        "ziti_rome_gnu_omp_no_simd_scheduling_dynamic_inc_OMP_updown": "Dynamic",
        "ziti_rome_gnu_omp_no_simd_scheduling_guided_inc_OMP_updown": "Guided",
        "ziti_rome_gnu_omp_no_simd_scheduling_auto_inc_OMP_updown": "Auto",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="ziti-rome OpenMP Updown Inclusive Scan with Different Scheduling (gnu)",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


def figure_tiled_scheduling_rome(plotwrapper):
    columns = {
        "ziti_rome_gnu_omp_no_simd_scheduling_static_inc_OMP_tiled": "Static",
        "ziti_rome_gnu_omp_no_simd_scheduling_dynamic_inc_OMP_tiled": "Dynamic",
        "ziti_rome_gnu_omp_no_simd_scheduling_guided_inc_OMP_tiled": "Guided",
        "ziti_rome_gnu_omp_no_simd_scheduling_auto_inc_OMP_tiled": "Auto",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="ziti-rome OpenMP Tiled Inclusive Scan with Different Scheduling (gnu)",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


# ---------------------------------------------------------------------------------
# OpenMP Scheduling no SIMD with icx Plots
# ---------------------------------------------------------------------------------


def figure_updown_scheduling_icx_media(plotwrapper):
    columns = {
        "mp_media_icx_omp_no_simd_scheduling_static_inc_OMP_updown": "Static",
        "mp_media_icx_omp_no_simd_scheduling_dynamic_inc_OMP_updown": "Dynamic",
        "mp_media_icx_omp_no_simd_scheduling_guided_inc_OMP_updown": "Guided",
        "mp_media_icx_omp_no_simd_scheduling_auto_inc_OMP_updown": "Auto",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="mp-media OpenMP Updown Inclusive Scans with Different Scheduling (icx)",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


def figure_tiled_scheduling_icx_media(plotwrapper):
    columns = {
        "mp_media_icx_omp_no_simd_scheduling_static_inc_OMP_tiled": "Static",
        "mp_media_icx_omp_no_simd_scheduling_dynamic_inc_OMP_tiled": "Dynamic",
        "mp_media_icx_omp_no_simd_scheduling_guided_inc_OMP_tiled": "Guided",
        "mp_media_icx_omp_no_simd_scheduling_auto_inc_OMP_tiled": "Auto",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="mp-media OpenMP Tiled Inclusive Scans with Different Scheduling (icx)",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


# ---------------------------------------------------------------------------------
# OpenMP Scheduling SIMD with icx Plots
# ---------------------------------------------------------------------------------


def figure_updown_scheduling_SIMD_icx_media(plotwrapper):
    columns = {
        "mp_media_icx_omp_scheduling_static_inc_OMP_updown": "Static",
        "mp_media_icx_omp_scheduling_dynamic_inc_OMP_updown": "Dynamic",
        "mp_media_icx_omp_scheduling_guided_inc_OMP_updown": "Guided",
        "mp_media_icx_omp_scheduling_auto_inc_OMP_updown": "Auto",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="mp-media OpenMP Updown Inclusive Scans with Different Scheduling and SIMD (icx)",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


# ---------------------------------------------------------------------------------
# Tile Size Optimized Plots
# ---------------------------------------------------------------------------------


def figure_tiledopt(plotwrapper):
    columns = {
        "gnu-algopt_inc_OMP_tiled": "Inclusive",
        "gnu-tiled_inc_OMP_tiled": "Inclusive Optimized",
        "gnu-algopt_ex_OMP_tiled": "Exclusive",
        "gnu-tiled_ex_OMP_tiled": "Exclusive Optimized",
    }
    for system in systems:
        syscol = dict()

        for key, value in columns.items():
            syscol[system + "-" + key] = value

        plotwrapper.columns = syscol
        plotwrapper.plot(
            title=system + " Tiled Benchmark Optimized",
            index_col="N",
            xscale="log",
            yscale="linear",
            grid=True,
        )


def figure_tiledopt_seg(plotwrapper):
    columns = {
        "gnu-algopt_incseg_OMP_tiled": "Inclusive",
        "gnu-tiled_incseg_OMP_tiled": "Inclusive Optimized",
        "gnu-algopt_exseg_OMP_tiled": "Exclusive",
        "gnu-tiled_exseg_OMP_tiled": "Exclusive Optimized",
    }
    for system in systems:
        syscol = dict()

        for key, value in columns.items():
            syscol[system + "-" + key] = value

        plotwrapper.columns = syscol
        plotwrapper.plot(
            title=system + " Tiled Benchmark Optimized Segmented",
            index_col="N",
            xscale="log",
            yscale="linear",
            grid=True,
        )


# ---------------------------------------------------------------------------------
# Analytical Sequential Plots
# ---------------------------------------------------------------------------------


def figure_sequential(plotwrapper):
    columns = {
        "gnu-ana_ana_inc_seq_sequential": "STL Inc",
        "gnu-ana_ana_ex_seq_sequential": "STL Ex",
        "gnu-ana_ana_inc_seq_updown": "Up-Down Inc",
        "gnu-ana_ana_ex_seq_updown": "Up-Down Ex",
        "gnu-ana_ana_inc_seq_tiled": "Tiled Inc",
        "gnu-ana_ana_ex_seq_tiled": "Tiled Ex",
    }
    for system in systems:
        syscol = dict()

        for key, value in columns.items():
            syscol[system + "-" + key] = value

        plotwrapper.columns = syscol
        plotwrapper.plot(
            title=system + " Analytical Sequential Scans",
            index_col="N",
            xscale="log",
            yscale="linear",
            grid=True,
        )


def figure_sequential_segmented(plotwrapper):
    columns = {
        "gnu-ana_ana_incseg_seq_sequential": "STL Inc",
        "gnu-ana_ana_exseg_seq_sequential": "STL Ex",
        "gnu-ana_ana_incseg_seq_updown": "Up-Down Inc",
        "gnu-ana_ana_exseg_seq_updown": "Up-Down Ex",
        "gnu-ana_ana_incseg_seq_tiled": "Tiled Inc",
        "gnu-ana_ana_exseg_seq_tiled": "Tiled Ex",
    }
    for system in systems:
        syscol = dict()

        for key, value in columns.items():
            syscol[system + "-" + key] = value

        plotwrapper.columns = syscol
        plotwrapper.plot(
            title=system + " Analytical Sequential Segmented Scans",
            index_col="N",
            xscale="log",
            yscale="linear",
            grid=True,
        )


# ---------------------------------------------------------------------------------
# Analytical Parallel Plots
# ---------------------------------------------------------------------------------


def figure_parallel_media(plotwrapper):
    columns = {
        "gnu-ana_ana_inc_seq_sequential": "STL",
        "gnu-ana_ana_inc_OMP_provided": "OpenMP Provided Scan",
        "gnu-ana_ana_inc_TBB_provided": "TBB Provided Scan",
        "gnu-ana_ana_inc_OMP_updown": "OpenMP Up-Down Sweep",
        "gnu-ana_ana_inc_TBB_updown": "TBB Up-Down Sweep",
        "gnu-ana_ana_inc_OMP_tiled": "OpenMP Tiled Scan",
        "gnu-ana_ana_inc_TBB_tiled": "TBB Tiled Scan",
    }
    for system in systems:
        syscol = dict()

        for key, value in columns.items():
            syscol[system + "-" + key] = value

        plotwrapper.columns = syscol
        plotwrapper.plot(
            title=system + " Analytical Parallel Inclusive Scans",
            index_col="N",
            xscale="log",
            yscale="linear",
            grid=True,
        )


def figure_parallel_segmented(plotwrapper):
    columns = {
        "gnu-ana_ana_incseg_seq_sequential": "STL",
        "gnu-ana_ana_incseg_TBB_provided": "TBB Provided Scan",
        "gnu-ana_ana_incseg_OMP_updown": "OpenMP Up-Down Sweep",
        "gnu-ana_ana_incseg_TBB_updown": "TBB Up-Down Sweep",
        "gnu-ana_ana_incseg_OMP_tiled": "OpenMP Tiled Scan",
        "gnu-ana_ana_incseg_TBB_tiled": "TBB Tiled Scan",
    }
    for system in systems:
        syscol = dict()

        for key, value in columns.items():
            syscol[system + "-" + key] = value

        plotwrapper.columns = syscol
        plotwrapper.plot(
            title=system
            + " Analytical Parallel Inclusive Segmented Scans",
            index_col="N",
            xscale="log",
            yscale="linear",
            grid=True,
        )


# ---------------------------------------------------------------------------------
# Analytical Algorithmic Optimization Plots
# ---------------------------------------------------------------------------------


def figure_algopt_inclusive(plotwrapper):
    columns = {
        "gnu-ana_ana_inc_seq_sequential": "STL",
        "gnu-algopt-ana_ana_inc_seq_sequential": "AlgOpt STL",
        "gnu-ana_ana_inc_seq_updown": "Up-Down Sweep",
        "gnu-algopt-ana_ana_inc_seq_updown": "AlgOpt Up-Down Sweep",
        "gnu-ana_ana_inc_seq_tiled": "Tiled Scan",
        "gnu-algopt-ana_ana_inc_seq_tiled": "AlgOpt Tiled Scan",
    }
    for system in systems:
        syscol = dict()

        for key, value in columns.items():
            syscol[system + "-" + key] = value

        plotwrapper.columns = syscol
        plotwrapper.plot(
            title=system
            + " Analytical Alg. Optimized Sequential Inclusive Scans",
            index_col="N",
            xscale="log",
            yscale="linear",
            grid=True,
        )


def figure_algopt_exclusive_media(plotwrapper):
    columns = {
        "gnu-ana_ana_ex_seq_sequential": "Naive (STL)",
        "gnu-algopt-ana_ana_ex_seq_sequential": "AlgOpt Naive (STL)",
        "gnu-ana_ana_ex_seq_updown": "Up-Down Sweep",
        "gnu-algopt-ana_ana_ex_seq_updown": "AlgOpt Up-Down Sweep",
        "gnu-ana_ana_ex_seq_tiled": "Tiled Scan",
        "gnu-algopt-ana_ana_ex_seq_tiled": "AlgOpt Tiled Scan",
    }
    for system in systems:
        syscol = dict()

        for key, value in columns.items():
            syscol[system + "-" + key] = value

        plotwrapper.columns = syscol
        plotwrapper.plot(
            title=system
            + " Analytical Alg. Optimized Sequential Exclusive Scans",
            index_col="N",
            xscale="log",
            yscale="linear",
            grid=True,
        )


def figure_algopt_inclusive_segmented(plotwrapper):
    columns = {
        "gnu-ana_ana_incseg_seq_sequential": "Naive (STL)",
        "gnu-ana_ana_incseg_seq_updown": "Up-Down Sweep",
        "gnu-ana_ana_incseg_seq_tiled": "Tiled Scan",
        "gnu-algopt-ana_ana_incseg_seq_sequential": "AlgOpt Naive (STL)",
        "gnu-algopt-ana_ana_incseg_seq_updown": "AlgOpt Up-Down Sweep",
        "gnu-algopt-ana_ana_incseg_seq_tiled": "AlgOpt Tiled Scan",
    }
    for system in systems:
        syscol = dict()

        for key, value in columns.items():
            syscol[system + "-" + key] = value

        plotwrapper.columns = syscol
        plotwrapper.plot(
            title=system
            + " Analytical Alg. Optimized Sequential Segmented Inclusive Scans",
            index_col="N",
            xscale="log",
            yscale="linear",
            grid=True,
        )


def figure_algopt_exclusive_segmented(plotwrapper):
    columns = {
        "gnu-ana_ana_exseg_seq_sequential": "Naive (STL)",
        "gnu-ana_ana_exseg_seq_updown": "Up-Down Sweep",
        "gnu-ana_ana_exseg_seq_tiled": "Tiled Scan",
        "gnu-algopt-ana_ana_exseg_seq_sequential": "AlgOpt Naive (STL)",
        "gnu-algopt-ana_ana_exseg_seq_updown": "AlgOpt Up-Down Sweep",
        "gnu-algopt-ana_ana_exseg_seq_tiled": "AlgOpt Tiled Scan",
    }
    for system in systems:
        syscol = dict()

        for key, value in columns.items():
            syscol[system + "-" + key] = value

        plotwrapper.columns = syscol
        plotwrapper.plot(
            title=system
            + " Analytical Alg. Optimized Sequential Segmented Exclusive Scans",
            index_col="N",
            xscale="log",
            yscale="linear",
            grid=True,
        )
