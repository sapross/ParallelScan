#define CATCH_CONFIG_FAST_COMPILE
#include "csv_reporter.hpp"
#include "logrange_generator.hpp"
#include <catch2/catch.hpp>

#include "scan.hpp"

SCENARIO("Inclusive Scan", "[inc]")
{

    std::default_random_engine            generator;
    std::uniform_real_distribution<float> distribution(1., 10.);
    auto                                  rand = std::bind(distribution, generator);

    // Benchmark parameters
    const size_t N = GENERATE(logRange(1ull << 15, 1ull << 30, 2));

    // Logging of variables
    CAPTURE(N);
    SUCCEED();

    std::vector<float> data(N, 0.);
    std::generate(data.begin(), data.end(), rand);

    // Benchmark
    BENCHMARK_ADVANCED("Sequential Naive")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<float> result(N, 0);
        meter.measure(
            [N, &data, &result]() {
                naive::sequential::inclusive_scan(
                    data.begin(), data.end(), result.begin());
            });
    };
    BENCHMARK_ADVANCED("Sequential Up-Down")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<float> result(N, 0);
        meter.measure(
            [N, &data, &result]()
            { naive::updown::inclusive_scan(data.begin(), data.end(), result.begin()); });
    };
    BENCHMARK_ADVANCED("Sequential Tiled")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<float> result(N, 0);
        meter.measure(
            [N, &data, &result]()
            { naive::tiled::inclusive_scan(data.begin(), data.end(), result.begin()); });
    };
}
