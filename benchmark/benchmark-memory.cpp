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
#pragma omp parallel for schedule(static)
    for (size_t i = 0; i < data.size(); i++)
    {
        data[i] = rand();
    }

    // Benchmark
    BENCHMARK_ADVANCED("inc_seq_sequential")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure([&data]()
                      { sequential::naive::inclusive_scan(data.begin(), data.end()); });
    };
    BENCHMARK_ADVANCED("inc_seq_updown")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure([&data]()
                      { sequential::updown::inclusive_scan(data.begin(), data.end()); });
    };
    BENCHMARK_ADVANCED("inc_seq_tiled")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure([&data]()
                      { sequential::tiled::inclusive_scan(data.begin(), data.end()); });
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
#pragma omp parallel for schedule(static)
    for (size_t i = 0; i < data.size(); i++)
    {
        data[i] = rand();
    }

    BENCHMARK_ADVANCED("inc_OMP_provided")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure([&data]()
                      { openmp::provided::inclusive_scan(data.begin(), data.end()); });
    };
    BENCHMARK_ADVANCED("inc_OMP_updown")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure([&data]()
                      { openmp::updown::inclusive_scan(data.begin(), data.end()); });
    };
    BENCHMARK_ADVANCED("inc_OMP_tiled")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure([&data]()
                      { openmp::tiled::inclusive_scan(data.begin(), data.end()); });
    };
}

SCENARIO("Inclusive Scan TBB", "[inc] [tbb]")
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
#pragma omp parallel for schedule(static)
    for (size_t i = 0; i < data.size(); i++)
    {
        data[i] = rand();
    }

    BENCHMARK_ADVANCED("inc_TBB_provided")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure(
            [&data]() {
                _tbb::provided::inclusive_scan(
                    data.begin(), data.end(), 0.0, data.begin());
            });
    };

    BENCHMARK_ADVANCED("inc_TBB_updown")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure([&data]()
                      { _tbb::updown::inclusive_scan(data.begin(), data.end()); });
    };

    BENCHMARK_ADVANCED("inc_TBB_tiled")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure([&data]()
                      { _tbb::tiled::inclusive_scan(data.begin(), data.end()); });
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
#pragma omp parallel for schedule(static)
    for (size_t i = 0; i < data.size(); i++)
    {
        data[i] = rand();
    }

    float init = 0.0;

    // Benchmark
    BENCHMARK_ADVANCED("ex_seq_sequential")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure(
            [&data, init]()
            { sequential::naive::exclusive_scan(data.begin(), data.end(), init); });
    };

    BENCHMARK_ADVANCED("ex_seq_updown")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure(
            [&data, init]()
            { sequential::updown::exclusive_scan(data.begin(), data.end(), init); });
    };

    BENCHMARK_ADVANCED("ex_seq_tiled")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure(
            [&data, init]()
            { sequential::tiled::exclusive_scan(data.begin(), data.end(), init); });
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
#pragma omp parallel for schedule(static)
    for (size_t i = 0; i < data.size(); i++)
    {
        data[i] = rand();
    }

    float init = 0.0;

    BENCHMARK_ADVANCED("ex_OMP_provided")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure(
            [&data, init]()
            { openmp::provided::exclusive_scan(data.begin(), data.end(), init); });
    };

    BENCHMARK_ADVANCED("ex_OMP_updown")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure(
            [&data, init]()
            { openmp::updown::exclusive_scan(data.begin(), data.end(), init); });
    };

    BENCHMARK_ADVANCED("ex_OMP_tiled")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure([&data, init]()
                      { openmp::tiled::exclusive_scan(data.begin(), data.end(), init); });
    };
}
SCENARIO("Exclusive Scan TBB", "[ex] [tbb]")
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
#pragma omp parallel for schedule(static)
    for (size_t i = 0; i < data.size(); i++)
    {
        data[i] = rand();
    }

    float init = 0.0;

    BENCHMARK_ADVANCED("ex_TBB_provided")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure(
            [&data, init]()
            { _tbb::provided::exclusive_scan(data.begin(), data.end(), init); });
    };

    BENCHMARK_ADVANCED("ex_TBB_updown")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure([&data, init]()
                      { _tbb::updown::exclusive_scan(data.begin(), data.end(), init); });
    };

    BENCHMARK_ADVANCED("ex_TBB_tiled")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure([&data, init]()
                      { _tbb::tiled::exclusive_scan(data.begin(), data.end(), init); });
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
            [&data]()
            { sequential::naive::inclusive_segmented_scan(data.begin(), data.end()); });
    };

    BENCHMARK_ADVANCED("incseg_seq_updown")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [&data]()
            { sequential::updown::inclusive_segmented_scan(data.begin(), data.end()); });
    };

    BENCHMARK_ADVANCED("incseg_seq_tiled")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [&data]()
            { sequential::tiled::inclusive_segmented_scan(data.begin(), data.end()); });
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

    BENCHMARK_ADVANCED("incseg_OMP_updown")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [&data]()
            { openmp::updown::inclusive_segmented_scan(data.begin(), data.end()); });
    };

    BENCHMARK_ADVANCED("incseg_OMP_tiled")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [&data]()
            { openmp::tiled::inclusive_segmented_scan(data.begin(), data.end()); });
    };
}
SCENARIO("Inclusive Segmented Scan TBB", "[inc] [seg] [tbb]")
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

    BENCHMARK_ADVANCED("incseg_TBB_provided")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [&data]() {
                _tbb::provided::inclusive_segmented_scan(data.begin(), data.end(), 0.0f);
            });
    };

    BENCHMARK_ADVANCED("incseg_TBB_updown")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [&data]()
            { _tbb::updown::inclusive_segmented_scan(data.begin(), data.end()); });
    };

    BENCHMARK_ADVANCED("incseg_TBB_tiled")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [&data]()
            { _tbb::tiled::inclusive_segmented_scan(data.begin(), data.end()); });
    };
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
            [&data, init]() {
                sequential::naive::exclusive_segmented_scan(
                    data.begin(), data.end(), init);
            });
    };

    BENCHMARK_ADVANCED("exseg_seq_updown")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [&data, init]() {
                sequential::updown::exclusive_segmented_scan(
                    data.begin(), data.end(), .0f, init);
            });
    };

    BENCHMARK_ADVANCED("exseg_seq_tiled")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [&data, init]() {
                sequential::tiled::exclusive_segmented_scan(
                    data.begin(), data.end(), .0f, init);
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

    BENCHMARK_ADVANCED("exseg_OMP_updown")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [&data, init]() {
                openmp::updown::exclusive_segmented_scan(
                    data.begin(), data.end(), .0f, init);
            });
    };

    BENCHMARK_ADVANCED("exseg_OMP_tiled")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [&data, init]() {
                openmp::tiled::exclusive_segmented_scan(
                    data.begin(), data.end(), .0f, init);
            });
    };
}
SCENARIO("Exclusive Segmented Scan TBB", "[ex] [seg] [tbb]")
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

    BENCHMARK_ADVANCED("exseg_TBB_provided")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [&data, init]() {
                _tbb::provided::exclusive_segmented_scan(data.begin(), data.end(), init);
            });
    };

    BENCHMARK_ADVANCED("exseg_TBB_updown")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [&data, &result, init]()
            {
                _tbb::updown::exclusive_segmented_scan(
                    data.begin(), data.end(), result.begin(), 0.0f, init);
            });
    };

    BENCHMARK_ADVANCED("exseg_TBB_tiled")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [&data, init]()
            { _tbb::tiled::exclusive_segmented_scan(data.begin(), data.end(), init); });
    };
}

SCENARIO("Inclusive Scan Tile Size", "[.][tilesize]")
{

    std::default_random_engine            generator;
    std::uniform_real_distribution<float> distribution(1., 10.);
    auto                                  rand = std::bind(distribution, generator);

    // Benchmark parameters
    const size_t N         = 1ull << 30;
    const size_t tile_size = GENERATE(logRange(1ull << 4, 1ull << 28, 2));

    // Logging of variables
    CAPTURE(tile_size);
    CAPTURE(N);
    SUCCEED();

    std::vector<float> data(N);
#pragma omp parallel for schedule(static)
    for (size_t i = 0; i < data.size(); i++)
    {
        data[i] = rand();
    }

    // Benchmark
    BENCHMARK_ADVANCED("omp_outofplace_inc_tilesize")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<float> result(N);
        openmp::tiled::set_tile_size(tile_size);
        meter.measure([&data]()
                      { openmp::tiled::inclusive_scan(data.begin(), data.end()); });
    };

    BENCHMARK_ADVANCED("omp_inplace_inc_tilesize")(Catch::Benchmark::Chronometer meter)
    {
        openmp::tiled::set_tile_size(tile_size);
        meter.measure(
            [&data]()
            { openmp::tiled::inclusive_scan(data.begin(), data.end(), data.begin()); });
    };
}

SCENARIO("Inclusive Segmented Scan Tile Size", "[.][tilesize]")
{
    std::default_random_engine            generator;
    std::uniform_real_distribution<float> distribution(1., 10.);
    auto                                  rand = std::bind(distribution, generator);

    std::default_random_engine         flag_generator;
    std::uniform_int_distribution<int> flag_distribution(0, 1);
    auto flag_rand = std::bind(flag_distribution, flag_generator);

    // Benchmark parameters
    const size_t N         = 1ull << 30;
    const size_t tile_size = GENERATE(logRange(1ull << 4, 1ull << 28, 2));

    // Logging of variables
    CAPTURE(tile_size);
    CAPTURE(N);
    SUCCEED();

    std::vector<std::pair<float, int>> data(N);

#pragma omp parallel for schedule(static)
    for (size_t i = 0; i < N; i++)
    {
        data[i] = std::make_pair(rand(), flag_rand());
    }

    BENCHMARK_ADVANCED("omp_outofplace_incseg_tilesize")
    (Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        openmp::tiled::set_tile_size(tile_size);

        meter.measure(
            [&data]()
            { openmp::tiled::inclusive_segmented_scan(data.begin(), data.end()); });
    };

    BENCHMARK_ADVANCED("omp_inplace_incseg_tilesize")
    (Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        openmp::tiled::set_tile_size(tile_size);
        meter.measure(
            [&data]() {
                openmp::tiled::inclusive_segmented_scan(
                    data.begin(), data.end(), data.begin());
            });
    };
}
SCENARIO("Exclusive Segmented Scan Tile Size", "[.][tilesize]")
{
    std::default_random_engine            generator;
    std::uniform_real_distribution<float> distribution(1., 10.);
    auto                                  rand = std::bind(distribution, generator);

    std::default_random_engine         flag_generator;
    std::uniform_int_distribution<int> flag_distribution(0, 1);
    auto flag_rand = std::bind(flag_distribution, flag_generator);

    // Benchmark parameters
    const size_t N         = 1ull << 30;
    const size_t tile_size = GENERATE(logRange(1ull << 4, 1ull << 28, 2));

    // Logging of variables
    CAPTURE(tile_size);
    CAPTURE(N);
    SUCCEED();

    std::vector<std::pair<float, int>> data(N);
#pragma omp parallel for schedule(static)
    for (size_t i = 0; i < N; i++)
    {
        data[i] = std::make_pair(rand(), flag_rand());
    }

    BENCHMARK_ADVANCED("omp_outofplace_exseg_tilesize")
    (Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        openmp::tiled::set_tile_size(tile_size);

        meter.measure(
            [&data]()
            { openmp::tiled::exclusive_segmented_scan(data.begin(), data.end(), 0, 0); });
    };

    BENCHMARK_ADVANCED("omp_inplace_exseg_tilesize")
    (Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        openmp::tiled::set_tile_size(tile_size);
        meter.measure(
            [&data]()
            { openmp::tiled::exclusive_segmented_scan(data.begin(), data.end(), 0, 0); });
    };
}
