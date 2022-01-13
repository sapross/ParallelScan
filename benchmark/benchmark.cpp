#define CATCH_CONFIG_FAST_COMPILE
#include "csv_reporter.hpp"
#include "logrange_generator.hpp"
#include <catch2/catch.hpp>

#include "scan.hpp"

SCENARIO("Inclusive Scan Sequential", "[inc] [seq]")
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
    BENCHMARK_ADVANCED("inc_seq_sequential")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<float> result(N, 0);
        meter.measure(
            [&data, &result]() {
                sequential::naive::inclusive_scan(
                    data.begin(), data.end(), result.begin());
            });
    };
    BENCHMARK_ADVANCED("inc_seq_updown")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<float> result(N, 0);
        meter.measure(
            [&data, &result]() {
                sequential::updown::inclusive_scan(
                    data.begin(), data.end(), result.begin());
            });
    };
    BENCHMARK_ADVANCED("inc_seq_tiled")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<float> result(N, 0);
        meter.measure(
            [&data, &result]() {
                sequential::tiled::inclusive_scan(
                    data.begin(), data.end(), result.begin());
            });
    };
}
SCENARIO("Inclusive Scan OpenMP", "[inc] [omp]")
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
    BENCHMARK_ADVANCED("inc_OMP_provided")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<float> result(N, 0);
        meter.measure(
            [&data, &result]() {
                openmp::provided::inclusive_scan(
                    data.begin(), data.end(), result.begin());
            });
    };
    BENCHMARK_ADVANCED("inc_OMP_updown")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<float> result(N, 0);
        meter.measure(
            [&data, &result]() {
                openmp::updown::inclusive_scan(data.begin(), data.end(), result.begin());
            });
    };
    BENCHMARK_ADVANCED("inc_OMP_tiled")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<float> result(N, 0);
        meter.measure(
            [&data, &result]()
            { openmp::tiled::inclusive_scan(data.begin(), data.end(), result.begin()); });
    };
}

SCENARIO("Exclusive Scan", "[ex] [seq]")
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
    BENCHMARK_ADVANCED("ex_seq_sequential")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<float> result(N, 0);
        meter.measure(
            [&data, &result, init]() {
                sequential::naive::exclusive_scan(
                    data.begin(), data.end(), result.begin(), init);
            });
    };
    BENCHMARK_ADVANCED("ex_seq_updown")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<float> result(N, 0);
        meter.measure(
            [&data, &result, init]() {
                sequential::updown::exclusive_scan(
                    data.begin(), data.end(), result.begin(), init);
            });
    };
    BENCHMARK_ADVANCED("ex_seq_tiled")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<float> result(N, 0);
        meter.measure(
            [&data, &result, init]() {
                sequential::tiled::exclusive_scan(
                    data.begin(), data.end(), result.begin(), init);
            });
    };
}
SCENARIO("Exclusive Scan OpenMP", "[ex] [omp]")
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

    BENCHMARK_ADVANCED("ex_OMP_provided")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<float> result(N, 0);
        meter.measure(
            [&data, &result, init]() {
                openmp::provided::exclusive_scan(
                    data.begin(), data.end(), result.begin(), init);
            });
    };
    BENCHMARK_ADVANCED("ex_OMP_updown")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<float> result(N, 0);
        meter.measure(
            [&data, &result, init]() {
                openmp::updown::exclusive_scan(
                    data.begin(), data.end(), result.begin(), init);
            });
    };
    BENCHMARK_ADVANCED("ex_OMP_tiled")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<float> result(N, 0);
        meter.measure(
            [&data, &result, init]() {
                openmp::tiled::exclusive_scan(
                    data.begin(), data.end(), result.begin(), init);
            });
    };
}

SCENARIO("Inclusive Segmented Scan Sequential", "[inc] [seg] [seq]")
{

    std::default_random_engine            generator;
    std::uniform_real_distribution<float> distribution(1., 10.);
    auto                                  rand = std::bind(distribution, generator);

    std::default_random_engine         flag_generator;
    std::uniform_int_distribution<int> flag_distribution(0, 1);
    auto flag_rand = std::bind(flag_distribution, flag_generator);

    // Benchmark parameters
    const size_t N = GENERATE(logRange(1ull << 15, 1ull << 30, 2));

    // Logging of variables
    CAPTURE(N);
    SUCCEED();

    std::vector<std::pair<float, int>> data(N);
    std::generate(data.begin(),
                  data.end(),
                  [&rand, &flag_rand]()
                  {
                      std::pair<float, int> A;
                      A.first  = rand();
                      A.second = flag_rand();
                      return A;
                  });

    // Benchmark
    BENCHMARK_ADVANCED("incseg_seq_sequential")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [&data, &result]()
            {
                sequential::naive::inclusive_segmented_scan(
                    data.begin(), data.end(), result.begin());
            });
    };
    BENCHMARK_ADVANCED("incseg_seq_updown")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [&data, &result]()
            {
                sequential::updown::inclusive_segmented_scan(
                    data.begin(), data.end(), result.begin());
            });
    };
    BENCHMARK_ADVANCED("incseg_seq_tiled")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [&data, &result]()
            {
                sequential::tiled::inclusive_segmented_scan(
                    data.begin(), data.end(), result.begin());
            });
    };
}
SCENARIO("Inclusive Segmented Scan OpenMP", "[inc] [seg] [omp]")
{

    std::default_random_engine            generator;
    std::uniform_real_distribution<float> distribution(1., 10.);
    auto                                  rand = std::bind(distribution, generator);

    std::default_random_engine         flag_generator;
    std::uniform_int_distribution<int> flag_distribution(0, 1);
    auto flag_rand = std::bind(flag_distribution, flag_generator);

    // Benchmark parameters
    const size_t N = GENERATE(logRange(1ull << 15, 1ull << 30, 2));

    // Logging of variables
    CAPTURE(N);
    SUCCEED();

    std::vector<std::pair<float, int>> data(N);
    std::generate(data.begin(),
                  data.end(),
                  [&rand, &flag_rand]()
                  {
                      std::pair<float, int> A;
                      A.first  = rand();
                      A.second = flag_rand();
                      return A;
                  });

    // BENCHMARK_ADVANCED("incseg_OMP_provided")(Catch::Benchmark::Chronometer meter)
    // {
    //     std::vector<std::pair<float, int>> result(N);
    //     meter.measure(
    //         [&data, &result]()
    //         {
    //             openmp::provided::inclusive_segmented_scan(
    //                 data.begin(), data.end(), result.begin());
    //         });
    // };
    // BENCHMARK_ADVANCED("incseg_OMP_updown")(Catch::Benchmark::Chronometer meter)
    // {
    //     std::vector<std::pair<float, int>> result(N);
    //     meter.measure(
    //         [&data, &result]() {
    //             openmp::updown::inclusive_segmented_scan(
    //                 data.begin(), data.end(), result.begin());
    //         });
    // };
    // BENCHMARK_ADVANCED("incseg_OMP_tiled")(Catch::Benchmark::Chronometer meter)
    // {
    //     std::vector<std::pair<float, int>> result(N);
    //     meter.measure(
    //         [&data, &result]() {
    //             openmp::tiled::inclusive_segmented_scan(
    //                 data.begin(), data.end(), result.begin());
    //         });
    // };
}

SCENARIO("Exclusive Segmented Scan Sequential", "[ex] [seg] [seq]")
{

    std::default_random_engine            generator;
    std::uniform_real_distribution<float> distribution(1., 10.);
    auto                                  rand = std::bind(distribution, generator);

    std::default_random_engine         flag_generator;
    std::uniform_int_distribution<int> flag_distribution(0, 1);
    auto flag_rand = std::bind(flag_distribution, flag_generator);

    // Benchmark parameters
    const size_t N = GENERATE(logRange(1ull << 15, 1ull << 30, 2));

    // Logging of variables
    CAPTURE(N);
    SUCCEED();

    std::vector<std::pair<float, int>> data(N);
    std::generate(data.begin(),
                  data.end(),
                  [&rand, &flag_rand]()
                  {
                      std::pair<float, int> A;
                      A.first  = rand();
                      A.second = flag_rand();
                      return A;
                  });

    float init = 0.0;

    // Benchmark
    BENCHMARK_ADVANCED("exseg_seq_sequential")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [&data, &result, init]()
            {
                sequential::naive::exclusive_segmented_scan(
                    data.begin(), data.end(), result.begin(), init);
            });
    };
    BENCHMARK_ADVANCED("exseg_seq_updown")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [&data, &result, init]()
            {
                sequential::updown::exclusive_segmented_scan(
                    data.begin(), data.end(), result.begin(), init);
            });
    };
    BENCHMARK_ADVANCED("exseg_seq_tiled")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [&data, &result, init]()
            {
                sequential::tiled::exclusive_segmented_scan(
                    data.begin(), data.end(), result.begin(), init);
            });
    };
}

SCENARIO("Exclusive Segmented Scan OpenMP", "[ex] [seg] [omp]")
{

    std::default_random_engine            generator;
    std::uniform_real_distribution<float> distribution(1., 10.);
    auto                                  rand = std::bind(distribution, generator);

    std::default_random_engine         flag_generator;
    std::uniform_int_distribution<int> flag_distribution(0, 1);
    auto flag_rand = std::bind(flag_distribution, flag_generator);

    // Benchmark parameters
    const size_t N = GENERATE(logRange(1ull << 15, 1ull << 30, 2));

    // Logging of variables
    CAPTURE(N);
    SUCCEED();

    std::vector<std::pair<float, int>> data(N);
    std::generate(data.begin(),
                  data.end(),
                  [&rand, &flag_rand]()
                  {
                      std::pair<float, int> A;
                      A.first  = rand();
                      A.second = flag_rand();
                      return A;
                  });

    float init = 0.0;

    // BENCHMARK_ADVANCED("exseg_OMP_provided")(Catch::Benchmark::Chronometer meter)
    // {
    //     std::vector<std::pair<float, int>> result(N);
    //     meter.measure(
    //         [&data, &result, init]()
    //         {
    //             openmp::provided::exclusive_segmented_scan(
    //                 data.begin(), data.end(), result.begin(), init);
    //         });
    // };
    // BENCHMARK_ADVANCED("exseg_OMP_updown")(Catch::Benchmark::Chronometer meter)
    // {
    //     std::vector<std::pair<float, int>> result(N);
    //     meter.measure(
    //         [&data, &result, init]()
    //         {
    //             openmp::updown::exclusive_segmented_scan(
    //                 data.begin(), data.end(), result.begin(), init);
    //         });
    // };
    // BENCHMARK_ADVANCED("exseg_OMP_tiled")(Catch::Benchmark::Chronometer meter)
    // {
    //     std::vector<std::pair<float, int>> result(N);
    //     meter.measure(
    //         [&data, &result, init]()
    //         {
    //             openmp::tiled::exclusive_segmented_scan(
    //                 data.begin(), data.end(), result.begin(), init);
    //         });
    // };
}
