#define CATCH_CONFIG_FAST_COMPILE
#include "csv_reporter.hpp"
#include "logrange_generator.hpp"
#include <catch2/catch.hpp>

#include "data.hpp"
#include "iterator.hpp"

#include "scan.hpp"

SCENARIO("Analytical Inclusive Scan Sequential", "[inc] [seq]")
{

    // Benchmark parameters
    const size_t N = GENERATE(logRange(1ull << 15, 1ull << 30, 2));

    // Logging of variables
    CAPTURE(N);
    SUCCEED();

    std::vector<bool, pad::default_init_allocator<bool>> data(N);
    std::fill(data.begin(), data.end(), false);

    pad::transform_iterator begin(0, [](ssize_t idx) { return (idx % 2) == true; });
    auto                    end = begin + N;

    auto binary_op = [](bool x, bool y) -> bool { return x ^ y; };

    // Benchmark
    BENCHMARK_ADVANCED("ana_inc_seq_sequential")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure(
            [begin, end, &binary_op, &data]()
            { sequential::naive::inclusive_scan(begin, end, data.begin(), binary_op); });
    };
    BENCHMARK_ADVANCED("ana_inc_seq_updown")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure(
            [begin, end, &binary_op, &data]()
            { sequential::updown::inclusive_scan(begin, end, data.begin(), binary_op); });
    };
    BENCHMARK_ADVANCED("ana_inc_seq_tiled")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure(
            [begin, end, &binary_op, &data]()
            { sequential::tiled::inclusive_scan(begin, end, data.begin(), binary_op); });
    };
}
/*
SCENARIO("Analytical Inclusive Scan OpenMP", "[inc] [omp]")
{

    // Benchmark parameters
    const size_t N = GENERATE(logRange(1ull << 15, 1ull << 30, 2));

    // Logging of variables
    CAPTURE(N);
    SUCCEED();

    std::vector<bool, pad::default_init_allocator<bool>> data(N);
    std::fill(data.begin(), data.end(), false);

    pad::transform_iterator begin(0, [](ssize_t idx) { return (idx % 2) == true; });
    auto                    end = begin + N;

    auto binary_op = [](bool x, bool y) -> bool { return x ^ y; };

    BENCHMARK_ADVANCED("ana_inc_OMP_provided")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure([&data]()
                      { openmp::provided::inclusive_scan(data.begin(), data.end()); });
    };
    BENCHMARK_ADVANCED("ana_inc_OMP_updown")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure([&data]()
                      { openmp::updown::inclusive_scan(data.begin(), data.end()); });
    };
    BENCHMARK_ADVANCED("ana_inc_OMP_tiled")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure([&data]()
                      { openmp::tiled::inclusive_scan(data.begin(), data.end()); });
    };
}
///*
SCENARIO("Analytical Inclusive Scan TBB", "[inc] [tbb]")
{

    // Benchmark parameters
    const size_t N = GENERATE(logRange(1ull << 15, 1ull << 30, 2));

    // Logging of variables
    CAPTURE(N);
    SUCCEED();

    std::vector<bool, pad::default_init_allocator<bool>> data(N);
    std::fill(data.begin(), data.end(), false);

    pad::transform_iterator begin(0, [](ssize_t idx) { return (idx % 2) == true; });
    auto                    end = begin + N;

    auto binary_op = [](bool x, bool y) -> bool { return x ^ y; };


    BENCHMARK_ADVANCED("ana_inc_TBB_provided")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure(
            [&data]() {
                _tbb::provided::inclusive_scan(
                    data.begin(), data.end(), 0.0, data.begin());
            });
    };

    BENCHMARK_ADVANCED("ana_inc_TBB_updown")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure([&data]()
                      { _tbb::updown::inclusive_scan(data.begin(), data.end()); });
    };

    BENCHMARK_ADVANCED("ana_inc_TBB_tiled")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure([&data]()
                      { _tbb::tiled::inclusive_scan(data.begin(), data.end()); });
    };
}

SCENARIO("Analytical Exclusive Scan", "[ex] [seq]")
{

    // Benchmark parameters
    const size_t N = GENERATE(logRange(1ull << 15, 1ull << 30, 2));

    // Logging of variables
    CAPTURE(N);
    SUCCEED();

    std::vector<float> data(N, 0.);
    std::generate(data.begin(), data.end(), rand);
    float init = 0.0;

    // Benchmark
    BENCHMARK_ADVANCED("ana_ex_seq_sequential")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure(
            [&data, init]()
            { sequential::naive::exclusive_scan(data.begin(), data.end(), init); });
    };

    BENCHMARK_ADVANCED("ana_ex_seq_updown")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure(
            [&data, init]()
            { sequential::updown::exclusive_scan(data.begin(), data.end(), init); });
    };

    BENCHMARK_ADVANCED("ana_ex_seq_tiled")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure(
            [&data, init]()
            { sequential::tiled::exclusive_scan(data.begin(), data.end(), init); });
    };
}
SCENARIO("Analytical Exclusive Scan OpenMP", "[ex] [omp]")
{

    // Benchmark parameters
    const size_t N = GENERATE(logRange(1ull << 15, 1ull << 30, 2));

    // Logging of variables
    CAPTURE(N);
    SUCCEED();

    std::vector<float> data(N, 0.);
    std::generate(data.begin(), data.end(), rand);
    float init = 0.0;

    BENCHMARK_ADVANCED("ana_ex_OMP_provided")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure(
            [&data, init]()
            { openmp::provided::exclusive_scan(data.begin(), data.end(), init); });
    };

    BENCHMARK_ADVANCED("ana_ex_OMP_updown")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure(
            [&data, init]()
            { openmp::updown::exclusive_scan(data.begin(), data.end(), init); });
    };

    BENCHMARK_ADVANCED("ana_ex_OMP_tiled")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure([&data, init]()
                      { openmp::tiled::exclusive_scan(data.begin(), data.end(), init); });
    };
}
SCENARIO("Analytical Exclusive Scan TBB", "[ex] [tbb]")
{

    // Benchmark parameters
    const size_t N = GENERATE(logRange(1ull << 15, 1ull << 30, 2));

    // Logging of variables
    CAPTURE(N);
    SUCCEED();

    std::vector<float> data(N, 0.);
    std::generate(data.begin(), data.end(), rand);
    float init = 0.0;

    BENCHMARK_ADVANCED("ana_ex_TBB_provided")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure(
            [&data, init]()
            { _tbb::provided::exclusive_scan(data.begin(), data.end(), init); });
    };

    BENCHMARK_ADVANCED("ana_ex_TBB_updown")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure([&data, init]()
                      { _tbb::updown::exclusive_scan(data.begin(), data.end(), init); });
    };

    BENCHMARK_ADVANCED("ana_ex_TBB_tiled")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure([&data, init]()
                      { _tbb::tiled::exclusive_scan(data.begin(), data.end(), init); });
    };
}

SCENARIO("Analytical Inclusive Segmented Scan Sequential", "[inc] [seg] [seq]")
{

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
    BENCHMARK_ADVANCED("ana_incseg_seq_sequential")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [&data]()
            { sequential::naive::inclusive_segmented_scan(data.begin(), data.end()); });
    };

    BENCHMARK_ADVANCED("ana_incseg_seq_updown")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [&data]()
            { sequential::updown::inclusive_segmented_scan(data.begin(), data.end()); });
    };

    BENCHMARK_ADVANCED("ana_incseg_seq_tiled")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [&data]()
            { sequential::tiled::inclusive_segmented_scan(data.begin(), data.end()); });
    };
}
SCENARIO("Analytical Inclusive Segmented Scan OpenMP", "[inc] [seg] [omp]")
{

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

    BENCHMARK_ADVANCED("ana_incseg_OMP_updown")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [&data]()
            { openmp::updown::inclusive_segmented_scan(data.begin(), data.end()); });
    };

    BENCHMARK_ADVANCED("ana_incseg_OMP_tiled")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [&data]()
            { openmp::tiled::inclusive_segmented_scan(data.begin(), data.end()); });
    };
}
SCENARIO("Analytical Inclusive Segmented Scan TBB", "[inc] [seg] [tbb]")
{

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

    BENCHMARK_ADVANCED("ana_incseg_TBB_provided")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [&data]() {
                _tbb::provided::inclusive_segmented_scan(data.begin(), data.end(), 0.0f);
            });
    };

    BENCHMARK_ADVANCED("ana_incseg_TBB_updown")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [&data]()
            { _tbb::updown::inclusive_segmented_scan(data.begin(), data.end()); });
    };

    BENCHMARK_ADVANCED("ana_incseg_TBB_tiled")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [&data]()
            { _tbb::tiled::inclusive_segmented_scan(data.begin(), data.end()); });
    };
}

SCENARIO("Analytical Exclusive Segmented Scan Sequential", "[ex] [seg] [seq]")
{

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
    BENCHMARK_ADVANCED("ana_exseg_seq_sequential")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [&data, init]() {
                sequential::naive::exclusive_segmented_scan(
                    data.begin(), data.end(), init);
            });
    };

    BENCHMARK_ADVANCED("ana_exseg_seq_updown")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [&data, init]() {
                sequential::updown::exclusive_segmented_scan(
                    data.begin(), data.end(), .0f, init);
            });
    };

    BENCHMARK_ADVANCED("ana_exseg_seq_tiled")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [&data, init]() {
                sequential::tiled::exclusive_segmented_scan(
                    data.begin(), data.end(), .0f, init);
            });
    };
}

SCENARIO("Analytical Exclusive Segmented Scan OpenMP", "[ex] [seg] [omp]")
{

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

    BENCHMARK_ADVANCED("ana_exseg_OMP_updown")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [&data, init]() {
                openmp::updown::exclusive_segmented_scan(
                    data.begin(), data.end(), .0f, init);
            });
    };

    BENCHMARK_ADVANCED("ana_exseg_OMP_tiled")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [&data, init]() {
                openmp::tiled::exclusive_segmented_scan(
                    data.begin(), data.end(), .0f, init);
            });
    };
}
SCENARIO("Analytical Exclusive Segmented Scan TBB", "[ex] [seg] [tbb]")
{

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

    BENCHMARK_ADVANCED("ana_exseg_TBB_provided")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [&data, init]() {
                _tbb::provided::exclusive_segmented_scan(data.begin(), data.end(), init);
            });
    };

    BENCHMARK_ADVANCED("ana_exseg_TBB_updown")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [&data, init]()
            { _tbb::updown::exclusive_segmented_scan(data.begin(), data.end(), init); });
    };

    BENCHMARK_ADVANCED("ana_exseg_TBB_tiled")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [&data, init]()
            { _tbb::tiled::exclusive_segmented_scan(data.begin(), data.end(), init); });
    };
}

SCENARIO("Analytical Inclusive Scan Tile Size", "[.][tilesize]")
{

    // Benchmark parameters
    const size_t N         = 1ull << 30;
    const size_t tile_size = GENERATE(logRange(1ull << 4, 1ull << 28, 2));

    // Logging of variables
    CAPTURE(tile_size);
    CAPTURE(N);
    SUCCEED();

    std::vector<float> data(N);
    std::generate(data.begin(), data.end(), rand);

    // Benchmark
    BENCHMARK_ADVANCED("ana_omp_outofplace_inc_tilesize")
    (Catch::Benchmark::Chronometer meter)
    {
        std::vector<float> result(N);
        openmp::tiled::set_tile_size(tile_size);
        meter.measure([&data]()
                      { openmp::tiled::inclusive_scan(data.begin(), data.end()); });
    };

    BENCHMARK_ADVANCED("ana_omp_inplace_inc_tilesize")
    (Catch::Benchmark::Chronometer meter)
    {
        openmp::tiled::set_tile_size(tile_size);
        meter.measure(
            [&data]()
            { openmp::tiled::inclusive_scan(data.begin(), data.end(), data.begin()); });
    };
}

SCENARIO("Analytical Inclusive Segmented Scan Tile Size", "[.][tilesize]")
{

    // Benchmark parameters
    const size_t N         = 1ull << 30;
    const size_t tile_size = GENERATE(logRange(1ull << 4, 1ull << 28, 2));

    // Logging of variables
    CAPTURE(tile_size);
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

    BENCHMARK_ADVANCED("ana_omp_outofplace_incseg_tilesize")
    (Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        openmp::tiled::set_tile_size(tile_size);

        meter.measure(
            [&data]()
            { openmp::tiled::inclusive_segmented_scan(data.begin(), data.end()); });
    };

    BENCHMARK_ADVANCED("ana_omp_inplace_incseg_tilesize")
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
SCENARIO("Analytical Exclusive Segmented Scan Tile Size", "[.][tilesize]")
{

    // Benchmark parameters
    const size_t N         = 1ull << 30;
    const size_t tile_size = GENERATE(logRange(1ull << 4, 1ull << 28, 2));

    // Logging of variables
    CAPTURE(tile_size);
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

    BENCHMARK_ADVANCED("ana_omp_outofplace_exseg_tilesize")
    (Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        openmp::tiled::set_tile_size(tile_size);

        meter.measure(
            [&data]()
            { openmp::tiled::exclusive_segmented_scan(data.begin(), data.end(), 0, 0); });
    };

    BENCHMARK_ADVANCED("ana_omp_inplace_exseg_tilesize")
    (Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        openmp::tiled::set_tile_size(tile_size);
        meter.measure(
            [&data]()
            { openmp::tiled::exclusive_segmented_scan(data.begin(), data.end(), 0, 0); });
    };
}
*/
