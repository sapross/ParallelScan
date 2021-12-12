#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <csv_reporter.hpp>
#include <logrange_generator.hpp>

#include "scan.hpp"

SCENARIO("Test Template")
{
    //Benchmark parameters
    const size_t N = GENERATE(1, 2, 4, 8, 12, 16, 24, 32, 40, 48);
    

    // Logging of variables
    CAPTURE(N);
    SUCCEED();

    //Benchmark
    BENCHMARK_ADVANCED("benchname")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure([N](){ return N;});
    };
}

