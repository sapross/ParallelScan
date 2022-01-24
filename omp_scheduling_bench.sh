# Run specified benchmarks #

# Run all openmMP implementations with different scheduling

export OMP_SCHEDULE="static"
./build/bench -s -r csv [omp] >> documentation/results/omp_scheduling_static.csv
export OMP_SCHEDULE="dynamic"
./build/bench -s -r csv [omp] >> documentation/results/omp_scheduling_dynamic.csv
export OMP_SCHEDULE="guided"
./build/bench -s -r csv [omp] >> documentation/results/omp_scheduling_guided.csv
export OMP_SCHEDULE="auto"
./build/bench -s -r csv [omp] >> documentation/results/omp_scheduling_auto.csv