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
    BENCHMARK_ADVANCED("inc_seq_naive")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<float> result(N, 0);
        meter.measure(
            [N, &data, &result]() {
                naive::sequential::inclusive_scan(
                    data.begin(), data.end(), result.begin());
            });
    };
    BENCHMARK_ADVANCED("inc_seq_updown")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<float> result(N, 0);
        meter.measure(
            [N, &data, &result]()
            { naive::updown::inclusive_scan(data.begin(), data.end(), result.begin()); });
    };
    BENCHMARK_ADVANCED("inc_seq_tiled")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<float> result(N, 0);
        meter.measure(
            [N, &data, &result]()
            { naive::tiled::inclusive_scan(data.begin(), data.end(), result.begin()); });
    };
    BENCHMARK_ADVANCED("inc_OMP_provided")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<float> result(N, 0);
        meter.measure(
            [N, &data, &result]() {
                openmp::provided::inclusive_scan(
                    data.begin(), data.end(), result.begin());
            });
    };
    BENCHMARK_ADVANCED("inc_OMP_updown")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<float> result(N, 0);
        meter.measure(
            [N, &data, &result]() {
                openmp::updown::inclusive_scan(data.begin(), data.end(), result.begin());
            });
    };
    BENCHMARK_ADVANCED("inc_OMP_tiled")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<float> result(N, 0);
        meter.measure(
            [N, &data, &result]()
            { openmp::tiled::inclusive_scan(data.begin(), data.end(), result.begin()); });
    };
}
SCENARIO("Exclusive Scan", "[ex]")
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
    float init = 0.0;

    // Benchmark
    BENCHMARK_ADVANCED("ex_seq_naive")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<float> result(N, 0);
        meter.measure(
            [N, &data, &result, init]() {
                naive::sequential::exclusive_scan(
                    data.begin(), data.end(), result.begin(), init);
            });
    };
    BENCHMARK_ADVANCED("ex_seq_updown")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<float> result(N, 0);
        meter.measure(
            [N, &data, &result, init]() {
                naive::updown::exclusive_scan(
                    data.begin(), data.end(), result.begin(), init);
            });
    };
    BENCHMARK_ADVANCED("ex_seq_tiled")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<float> result(N, 0);
        meter.measure(
            [N, &data, &result, init]() {
                naive::tiled::exclusive_scan(
                    data.begin(), data.end(), result.begin(), init);
            });
    };
    BENCHMARK_ADVANCED("ex_OMP_provided")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<float> result(N, 0);
        meter.measure(
            [N, &data, &result, init]() {
                openmp::provided::exclusive_scan(
                    data.begin(), data.end(), result.begin(), init);
            });
    };
    BENCHMARK_ADVANCED("ex_OMP_updown")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<float> result(N, 0);
        meter.measure(
            [N, &data, &result, init]() {
                openmp::updown::exclusive_scan(
                    data.begin(), data.end(), result.begin(), init);
            });
    };
    BENCHMARK_ADVANCED("ex_OMP_tiled")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<float> result(N, 0);
        meter.measure(
            [N, &data, &result, init]() {
                openmp::tiled::exclusive_scan(
                    data.begin(), data.end(), result.begin(), init);
            });
    };
}
