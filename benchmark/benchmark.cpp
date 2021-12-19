#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <csv_reporter.hpp>
#include <logrange_generator.hpp>

#include "scan.hpp"

SCENARIO("Inclusive Scan", "[inc]")
{

    std::default_random_engine            generator;
    std::uniform_real_distribution<float> distribution(1., 10.);
    auto                                  rand = std::bind(distribution, generator);

    // Benchmark parameters
    const size_t N = LogRange(1024 * 32, 1024 * 1024 * 1024, 2);

    // Logging of variables
    CAPTURE(N);
    SUCCEED();

    const std::vector<float> data(N, 0.);
    std::generate(data.begin(), data.end(), rand);

    // Benchmark
    BENCHMARK_ADVANCED("Naive Sequential")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<float> result(N, 0);
        meter.measure(
            [N]() {
                naive::sequential::inclusive_scan(
                    data.begin(), data.end(), result.begin());
            });
    };
}
