
# Table of Contents

1.  [Tiled, Updown Parallel Scan in OpenMP and TBB](#org1586fa1)
    1.  [Documentation](#org2059401)
    2.  [Building and Executing](#org999594a)
    3.  [Plot Generation](#org5f205e2)


<a id="org1586fa1"></a>

# Tiled, Updown Parallel Scan in OpenMP and TBB

Implementation of the parallel scan algorithms &ldquo;Tiled&rdquo; and &ldquo;UpDown&rdquo; OpenMP and TBB with benchmarks to compare all versions with sequential and native implementations of the scan. Tests and benchmarks make use of the [Catch2](https://github.com/catchorg/Catch2) header-only testing framework. While all algorithms are optimized for performance, many compromises had to be made to remain close to the function signatures of the STL.

Project was completed as part of the lecture Parallel Algorithm Design in the winter semester at Heidelberg University.


<a id="org2059401"></a>

## Documentation

For an overview of the implemented algorithms visit `documentation/presentation.pdf`.


<a id="org999594a"></a>

## Building and Executing

To build our solution of the exercise, \`cd\` to the exercise directory and run

    bash build.sh

To run the possible tests described in \`test\test.cpp\` invoke

    ./build/test

For the benchmark run

    ./build/bench -s -r csv

additionally, a tag of the desired test may be appended to filter the tests.


<a id="org5f205e2"></a>

## Plot Generation

This project features automatic generation of plots defined in `documentation/plots.py` using `documentation/makegraph.py`
To generate all plots from the collected benchmark data run

    cd documentation
    python makeplot.py filedir "results" plotall

