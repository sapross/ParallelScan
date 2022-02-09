#!/bin/bash

export OMP_SCHEDULE="static"
./build/bench-memory-rome -s -r csv --benchmark-samples=20 [omp] >> documentation/results/ziti_rome_gnu_omp_scheduling_static.csv
export OMP_SCHEDULE="dynamic"
./build/bench-memory-rome -s -r csv --benchmark-samples=20 [omp] >> documentation/results/ziti_rome_gnu_omp_scheduling_dynamic.csv
export OMP_SCHEDULE="guided"
./build/bench-memory-rome -s -r csv --benchmark-samples=20 [omp] >> documentation/results/ziti_rome_gnu_omp_scheduling_guided.csv
export OMP_SCHEDULE="auto"
./build/bench-memory-rome -s -r csv --benchmark-samples=20 [omp] >> documentation/results/ziti_rome_gnu_omp_scheduling_auto.csv