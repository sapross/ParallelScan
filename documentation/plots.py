def figure01(plotwrapper):
    columns = {
        "mp-media-gnu_inc_seq_sequential": "STL Inc",
        "mp-media-gnu_ex_seq_sequential": "STL Ex",
        "mp-media-gnu_inc_seq_updown": "Up-Down Inc",
        "mp-media-gnu_ex_seq_updown": "Up-Down Ex",
        "mp-media-gnu_inc_seq_tiled": "Tiled Inc",
        "mp-media-gnu_ex_seq_tiled": "Tiled Ex",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="Mp-Media Sequential Scans",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


def figure02(plotwrapper):
    columns = {
        "mp-media-gnu_incseg_seq_sequential": "STL Inc",
        "mp-media-gnu_exseg_seq_sequential": "STL Ex",
        "mp-media-gnu_incseg_seq_updown": "Up-Down Inc",
        "mp-media-gnu_exseg_seq_updown": "Up-Down Ex",
        "mp-media-gnu_incseg_seq_tiled": "Tiled Inc",
        "mp-media-gnu_exseg_seq_tiled": "Tiled Ex",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="Mp-Media Sequential Segmented Scans",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


def figure03(plotwrapper):
    columns = {
        "mp-media-gnu_inc_seq_sequential": "STL",
        "mp-media-gnu_inc_OMP_provided": "OpenMP Provided Scan",
        "mp-media-gnu_inc_TBB_provided": "TBB Provided Scan",
        "mp-media-gnu_inc_OMP_updown": "OpenMP Up-Down Sweep",
        "mp-media-gnu_inc_TBB_updown": "TBB Up-Down Sweep",
        "mp-media-gnu_inc_OMP_tiled": "OpenMP Tiled Scan",
        "mp-media-gnu_inc_TBB_tiled": "TBB Tiled Scan",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="Mp-Media Parallel Inclusive Scans",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


def figure04(plotwrapper):
    columns = {
        "mp-media-gnu_incseg_seq_sequential": "STL",
        "mp-media-gnu_incseg_TBB_provided": "TBB Provided Scan",
        "mp-media-gnu_incseg_OMP_updown": "OpenMP Up-Down Sweep",
        "mp-media-gnu_incseg_TBB_updown": "TBB Up-Down Sweep",
        "mp-media-gnu_incseg_OMP_tiled": "OpenMP Tiled Scan",
        "mp-media-gnu_incseg_TBB_tiled": "TBB Tiled Scan",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="Mp-Media Parallel Inclusive Segmented Scans",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


def figure07(plotwrapper):
    columns = {
        "mp-media-gnu_inc_seq_sequential": "STL",
        "mp-media-gnu-algopt_inc_seq_sequential": "AlgOpt STL",
        "mp-media-gnu_inc_seq_updown": "Up-Down Sweep",
        "mp-media-gnu-algopt_inc_seq_updown": "AlgOpt Up-Down Sweep",
        "mp-media-gnu_inc_seq_tiled": "Tiled Scan",
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
        "mp-media-gnu-algopt_ex_seq_sequential": "AlgOpt Naive (STL)",
        "mp-media-gnu_ex_seq_updown": "Up-Down Sweep",
        "mp-media-gnu-algopt_ex_seq_updown": "AlgOpt Up-Down Sweep",
        "mp-media-gnu_ex_seq_tiled": "Tiled Scan",
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
        "ziti-rome-gnu_inc_seq_sequential": "STL Inc",
        "ziti-rome-gnu_ex_seq_sequential": "STL Ex",
        "ziti-rome-gnu_inc_seq_updown": "Up-Down Inc",
        "ziti-rome-gnu_ex_seq_updown": "Up-Down Ex",
        "ziti-rome-gnu_inc_seq_tiled": "Tiled Inc",
        "ziti-rome-gnu_ex_seq_tiled": "Tiled Ex",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="Ziti-Rome Sequential Scans",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


def figure12(plotwrapper):
    columns = {
        "ziti-rome-gnu_incseg_seq_sequential": "STL Inc",
        "ziti-rome-gnu_exseg_seq_sequential": "STL Ex",
        "ziti-rome-gnu_incseg_seq_updown": "Up-Down Inc",
        "ziti-rome-gnu_exseg_seq_updown": "Up-Down Ex",
        "ziti-rome-gnu_incseg_seq_tiled": "Tiled Inc",
        "ziti-rome-gnu_exseg_seq_tiled": "Tiled Ex",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="Ziti-Rome Sequential Segmented Scans",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


def figure13(plotwrapper):
    columns = {
        "ziti-rome-gnu_inc_seq_sequential": "STL",
        "ziti-rome-gnu_inc_OMP_provided": "OpenMP Provided Scan",
        "ziti-rome-gnu_inc_TBB_provided": "TBB Provided Scan",
        "ziti-rome-gnu_inc_OMP_updown": "OpenMP Up-Down Sweep",
        "ziti-rome-gnu_inc_TBB_updown": "TBB Up-Down Sweep",
        "ziti-rome-gnu_inc_OMP_tiled": "OpenMP Tiled Scan",
        "ziti-rome-gnu_inc_TBB_tiled": "TBB Tiled Scan",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="Ziti-Rome Parallel Inclusive Scans",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


def figure14(plotwrapper):
    columns = {
        "ziti-rome-gnu_incseg_seq_sequential": "STL",
        "ziti-rome-gnu_incseg_TBB_provided": "TBB Provided Scan",
        "ziti-rome-gnu_incseg_OMP_updown": "OpenMP Up-Down Sweep",
        "ziti-rome-gnu_incseg_TBB_updown": "TBB Up-Down Sweep",
        "ziti-rome-gnu_incseg_OMP_tiled": "OpenMP Tiled Scan",
        "ziti-rome-gnu_incseg_TBB_tiled": "TBB Tiled Scan",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="Ziti-Rome Parallel Inclusive Segmented Scans",
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

def figure34(plotwrapper):
    columns = {
        "mp-media-gnu-tilesize_omp_outofplace_inc_tilesize": "Inclusive",
        "mp-media-gnu-tilesize_omp_outofplace_incseg_tilesize": "Inclusive Segmented",
        "mp-media-gnu-tilesize_omp_outofplace_exseg_tilesize": "Exclusive Segmented",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="OpenMP Tile Size Benchmark Out-Of-Place (mp-media)",
        index_col="tile_size",
        xscale="log",
        yscale="linear",
        grid=True,
    )


def figure35(plotwrapper):
    columns = {
        "mp-media-gnu-tilesize_omp_inplace_inc_tilesize": "Inclusive",
        "mp-media-gnu-tilesize_omp_inplace_incseg_tilesize": "Inclusive Segmented",
        "mp-media-gnu-tilesize_omp_inplace_exseg_tilesize": "Exclusive Segmented",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="OpenMP Tile Size Benchmark In-Place (mp-media)",
        index_col="tile_size",
        xscale="log",
        yscale="linear",
        grid=True,
    )


def figure36(plotwrapper):
    columns = {
        "ziti-rome-gnu-tilesize_omp_outofplace_inc_tilesize": "Inclusive",
        "ziti-rome-gnu-tilesize_omp_outofplace_incseg_tilesize": "Inclusive Segmented",
        "ziti-rome-gnu-tilesize_omp_outofplace_exseg_tilesize": "Exclusive Segmented",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="OpenMP Tile Size Benchmark Out-Of-Place (ziti-rome)",
        index_col="tile_size",
        xscale="log",
        yscale="linear",
        grid=True,
    )


def figure37(plotwrapper):
    columns = {
        "ziti-rome-gnu-tilesize_omp_inplace_inc_tilesize": "Inclusive",
        "ziti-rome-gnu-tilesize_omp_inplace_incseg_tilesize": "Inclusive Segmented",
        "ziti-rome-gnu-tilesize_omp_inplace_exseg_tilesize": "Exclusive Segmented",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="OpenMP Tile Size Benchmark In-Place (ziti-rome)",
        index_col="tile_size",
        xscale="log",
        yscale="linear",
        grid=True,
    )

def figure38(plotwrapper):
    columns = {
        "mp_media_gnu_omp_scheduling_static_media2_inc_OMP_provided": "Static",
        "mp_media_gnu_omp_scheduling_dynamic_media2_inc_OMP_provided": "Dynamic",
        "mp_media_gnu_omp_scheduling_guided_media2_inc_OMP_provided": "Guided",
        "mp_media_gnu_omp_scheduling_auto_media2_inc_OMP_provided": "Auto",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="MP-Media OpenMP Provided Inclusive Scan with Different Scheduling and SIMD (gnu)",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )

def figure39(plotwrapper):
    columns = {
        "mp_media_gnu_omp_scheduling_static_media2_inc_OMP_updown": "Static",
        "mp_media_gnu_omp_scheduling_dynamic_media2_inc_OMP_updown": "Dynamic",
        "mp_media_gnu_omp_scheduling_guided_media2_inc_OMP_updown": "Guided",
        "mp_media_gnu_omp_scheduling_auto_media2_inc_OMP_updown": "Auto",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="MP-Media OpenMP Updown Inclusive Scans \n with Different Scheduling and SIMD (gnu)",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )

def figure40(plotwrapper):
    columns = {
        "mp_media_gnu_omp_scheduling_static_media2_inc_OMP_tiled": "Static",
        "mp_media_gnu_omp_scheduling_dynamic_media2_inc_OMP_tiled": "Dynamic",
        "mp_media_gnu_omp_scheduling_guided_media2_inc_OMP_tiled": "Guided",
        "mp_media_gnu_omp_scheduling_auto_media2_inc_OMP_tiled": "Auto",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="MP-Media OpenMP Tiled Inclusive Scans with Different Scheduling and SIMD (gnu)",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )

def figure41(plotwrapper):
    columns = {
        "ziti_rome_gnu_omp_scheduling_static_inc_OMP_provided": "Static",
        "ziti_rome_gnu_omp_scheduling_dynamic_inc_OMP_provided": "Dynamic",
        "ziti_rome_gnu_omp_scheduling_guided_inc_OMP_provided": "Guided",
        "ziti_rome_gnu_omp_scheduling_auto_inc_OMP_provided": "Auto",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="Ziti-Rome OpenMP Provided Inclusive Scan with Different Scheduling and SIMD (gnu)",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )

def figure42(plotwrapper):
    columns = {
        "ziti_rome_gnu_omp_scheduling_static_inc_OMP_tiled": "SIMD",
        "ziti_rome_gnu_omp_no_simd_scheduling_static_inc_OMP_tiled": "No SIMD",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="Ziti-Rome OpenMP Tiled Inclusive Scan (gnu)",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )

def figure69(plotwrapper):
    columns = {
        "mp_media_gnu_omp_scheduling_static_media2_inc_OMP_tiled": "SIMD",
        "mp_media_gnu_omp_no_simd_scheduling_static_media2_inc_OMP_tiled": "No SIMD",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="MP-Media OpenMP Tiled Inclusive Scan (gnu)",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )

def figure43(plotwrapper):
    columns = {
        "ziti_rome_gnu_omp_scheduling_static_inc_OMP_tiled": "Static",
        "ziti_rome_gnu_omp_scheduling_dynamic_inc_OMP_tiled": "Dynamic",
        "ziti_rome_gnu_omp_scheduling_guided_inc_OMP_tiled": "Guided",
        "ziti_rome_gnu_omp_scheduling_auto_inc_OMP_tiled": "Auto",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="Ziti-Rome OpenMP Tiled Inclusive Scan with Different Scheduling and SIMD (gnu)",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )

def figure44(plotwrapper):
    columns = {
        "mp_media_gnu_omp_no_simd_scheduling_static_media2_inc_OMP_provided": "Static",
        "mp_media_gnu_omp_no_simd_scheduling_dynamic_media2_inc_OMP_provided": "Dynamic",
        "mp_media_gnu_omp_no_simd_scheduling_guided_media2_inc_OMP_provided": "Guided",
        "mp_media_gnu_omp_no_simd_scheduling_auto_media2_inc_OMP_provided": "Auto",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="MP-Media OpenMP Provided Inclusive Scan with Different Scheduling (gnu)",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )

def figure45(plotwrapper):
    columns = {
        "mp_media_gnu_omp_no_simd_scheduling_static_media2_inc_OMP_updown": "Static",
        "mp_media_gnu_omp_no_simd_scheduling_dynamic_media2_inc_OMP_updown": "Dynamic",
        "mp_media_gnu_omp_no_simd_scheduling_guided_media2_inc_OMP_updown": "Guided",
        "mp_media_gnu_omp_no_simd_scheduling_auto_media2_inc_OMP_updown": "Auto",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="MP-Media OpenMP Updown Inclusive Scans with Different Scheduling (gnu)",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )

def figure46(plotwrapper):
    columns = {
        "mp_media_gnu_omp_no_simd_scheduling_static_media2_inc_OMP_tiled": "Static",
        "mp_media_gnu_omp_no_simd_scheduling_dynamic_media2_inc_OMP_tiled": "Dynamic",
        "mp_media_gnu_omp_no_simd_scheduling_guided_media2_inc_OMP_tiled": "Guided",
        "mp_media_gnu_omp_no_simd_scheduling_auto_media2_inc_OMP_tiled": "Auto",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="MP-Media OpenMP Tiled Inclusive Scans with Different Scheduling (gnu)",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )

def figure47(plotwrapper):
    columns = {
        "ziti_rome_gnu_omp_no_simd_scheduling_static_inc_OMP_provided": "Static",
        "ziti_rome_gnu_omp_no_simd_scheduling_dynamic_inc_OMP_provided": "Dynamic",
        "ziti_rome_gnu_omp_no_simd_scheduling_guided_inc_OMP_provided": "Guided",
        "ziti_rome_gnu_omp_no_simd_scheduling_auto_inc_OMP_provided": "Auto",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="Ziti-Rome OpenMP Provided Inclusive Scan with Different Scheduling (gnu)",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )

def figure48(plotwrapper):
    columns = {
        "ziti_rome_gnu_omp_no_simd_scheduling_static_inc_OMP_updown": "Static",
        "ziti_rome_gnu_omp_no_simd_scheduling_dynamic_inc_OMP_updown": "Dynamic",
        "ziti_rome_gnu_omp_no_simd_scheduling_guided_inc_OMP_updown": "Guided",
        "ziti_rome_gnu_omp_no_simd_scheduling_auto_inc_OMP_updown": "Auto",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="Ziti-Rome OpenMP Updown Inclusive Scan with Different Scheduling (gnu)",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )

def figure49(plotwrapper):
    columns = {
        "ziti_rome_gnu_omp_no_simd_scheduling_static_inc_OMP_tiled": "Static",
        "ziti_rome_gnu_omp_no_simd_scheduling_dynamic_inc_OMP_tiled": "Dynamic",
        "ziti_rome_gnu_omp_no_simd_scheduling_guided_inc_OMP_tiled": "Guided",
        "ziti_rome_gnu_omp_no_simd_scheduling_auto_inc_OMP_tiled": "Auto",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="Ziti-Rome OpenMP Tiled Inclusive Scan with Different Scheduling (gnu)",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )

def figure51(plotwrapper):
    columns = {
        "mp_media_icx_omp_no_simd_scheduling_static_inc_OMP_updown": "Static",
        "mp_media_icx_omp_no_simd_scheduling_dynamic_inc_OMP_updown": "Dynamic",
        "mp_media_icx_omp_no_simd_scheduling_guided_inc_OMP_updown": "Guided",
        "mp_media_icx_omp_no_simd_scheduling_auto_inc_OMP_updown": "Auto",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="MP-Media OpenMP Updown Inclusive Scans with Different Scheduling (icx)",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )

def figure52(plotwrapper):
    columns = {
        "mp_media_icx_omp_no_simd_scheduling_static_inc_OMP_tiled": "Static",
        "mp_media_icx_omp_no_simd_scheduling_dynamic_inc_OMP_tiled": "Dynamic",
        "mp_media_icx_omp_no_simd_scheduling_guided_inc_OMP_tiled": "Guided",
        "mp_media_icx_omp_no_simd_scheduling_auto_inc_OMP_tiled": "Auto",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="MP-Media OpenMP Tiled Inclusive Scans with Different Scheduling (icx)",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )

def figure54(plotwrapper):
    columns = {
        "mp_media_icx_omp_scheduling_static_inc_OMP_updown": "Static",
        "mp_media_icx_omp_scheduling_dynamic_inc_OMP_updown": "Dynamic",
        "mp_media_icx_omp_scheduling_guided_inc_OMP_updown": "Guided",
        "mp_media_icx_omp_scheduling_auto_inc_OMP_updown": "Auto",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="MP-Media OpenMP Updown Inclusive Scans with Different Scheduling and SIMD (icx)",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


def figure_tiledopt_media(plotwrapper):
    columns = {
        "mp-media-gnu-algopt_inc_OMP_tiled": "Inclusive",
        "mp-media-gnu-tiled_inc_OMP_tiled": "Inclusive Optimized",
        "mp-media-gnu-algopt_ex_OMP_tiled": "Exclusive",
        "mp-media-gnu-tiled_ex_OMP_tiled": "Exclusive Optimized",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="Tiled Benchmark Optimized (mp-media)",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


def figure_tiledopt_seg_media(plotwrapper):
    columns = {
        "mp-media-gnu-algopt_incseg_OMP_tiled": "Inclusive",
        "mp-media-gnu-tiled_incseg_OMP_tiled": "Inclusive Optimized",
        "mp-media-gnu-algopt_exseg_OMP_tiled": "Exclusive",
        "mp-media-gnu-tiled_exseg_OMP_tiled": "Exclusive Optimized",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="Tiled Benchmark Optimized Segmented (mp-media)",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


def figure_tiledopt_rome(plotwrapper):
    columns = {
        "ziti-rome-gnu-algopt_inc_OMP_tiled": "Inclusive",
        "ziti-rome-gnu-algopt_ex_OMP_tiled": "Exclusive",
        "ziti-rome-gnu-tiled_inc_OMP_tiled": "Inclusive Optimized",
        "ziti-rome-gnu-tiled_ex_OMP_tiled": "Exclusive Optimized",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="Tiled Benchmark Optimized (ziti-rome)",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )


def figure_tiledopt_seg_rome(plotwrapper):
    columns = {
        "ziti-rome-gnu-algopt_incseg_OMP_tiled": "Inclusive",
        "ziti-rome-gnu-algopt_exseg_OMP_tiled": "Exclusive",
        "ziti-rome-gnu-tiled_incseg_OMP_tiled": "Inclusive Optimized",
        "ziti-rome-gnu-tiled_exseg_OMP_tiled": "Exclusive Optimized",
    }
    plotwrapper.columns = columns
    plotwrapper.plot(
        title="Tiled Benchmark Optimized Segmented (ziti-rome)",
        index_col="N",
        xscale="log",
        yscale="linear",
        grid=True,
    )
