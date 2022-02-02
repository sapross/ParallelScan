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

    std::vector<bool> data(N);
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

SCENARIO("Analytical Inclusive Scan OpenMP", "[inc] [omp]")
{

    // Benchmark parameters
    const size_t N = GENERATE(logRange(1ull << 15, 1ull << 30, 2));

    // Logging of variables
    CAPTURE(N);
    SUCCEED();

    std::vector<bool> data(N);
    std::fill(data.begin(), data.end(), false);

    pad::transform_iterator begin(0, [](ssize_t idx) { return (idx % 2) == true; });
    auto                    end = begin + N;

    auto binary_op = [](bool x, bool y) -> bool { return x ^ y; };

    BENCHMARK_ADVANCED("ana_inc_OMP_provided")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure([begin, end, &binary_op, &data]()
                      { openmp::provided::inclusive_scan(begin, end, data.begin()); });
    };
    BENCHMARK_ADVANCED("ana_inc_OMP_updown")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure(
            [begin, end, &binary_op, &data]()
            { openmp::updown::inclusive_scan(begin, end, data.begin(), binary_op); });
    };
    BENCHMARK_ADVANCED("ana_inc_OMP_tiled")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure(
            [begin, end, &binary_op, &data]()
            { openmp::tiled::inclusive_scan(begin, end, data.begin(), binary_op); });
    };
}

SCENARIO("Analytical Inclusive Scan TBB", "[inc] [tbb]")
{

    // Benchmark parameters
    const size_t N = GENERATE(logRange(1ull << 15, 1ull << 30, 2));

    // Logging of variables
    CAPTURE(N);
    SUCCEED();

    std::vector<bool> data(N);
    std::fill(data.begin(), data.end(), false);

    pad::transform_iterator begin(0, [](ssize_t idx) { return (idx % 2) == true; });
    auto                    end = begin + N;

    auto binary_op = [](bool x, bool y) -> bool { return x ^ y; };

    BENCHMARK_ADVANCED("ana_inc_TBB_provided")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure(
            [begin, end, &binary_op, &data]() {
                _tbb::provided::inclusive_scan(
                    begin, end, data.begin(), false, binary_op);
            });
    };

    BENCHMARK_ADVANCED("ana_inc_TBB_updown")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure(
            [begin, end, &binary_op, &data]()
            { _tbb::updown::inclusive_scan(begin, end, data.begin(), binary_op); });
    };

    BENCHMARK_ADVANCED("ana_inc_TBB_tiled")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure(
            [begin, end, &binary_op, &data]()
            { _tbb::tiled::inclusive_scan(begin, end, data.begin(), binary_op); });
    };
}

SCENARIO("Analytical Exclusive Scan", "[ex] [seq]")
{

    // Benchmark parameters
    const size_t N = GENERATE(logRange(1ull << 15, 1ull << 30, 2));

    // Logging of variables
    CAPTURE(N);
    SUCCEED();

    std::vector<bool> data(N);
    std::fill(data.begin(), data.end(), false);

    pad::transform_iterator begin(0, [](ssize_t idx) { return (idx % 2) == true; });
    auto                    end = begin + N;

    auto binary_op = [](bool x, bool y) -> bool { return x ^ y; };
    bool init      = false;

    // Benchmark
    BENCHMARK_ADVANCED("ana_ex_seq_sequential")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure(
            [begin, end, &binary_op, &data, init]() {
                sequential::naive::exclusive_scan(
                    begin, end, data.begin(), init, binary_op);
            });
    };

    BENCHMARK_ADVANCED("ana_ex_seq_updown")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure(
            [begin, end, &binary_op, &data, init]() {
                sequential::updown::exclusive_scan(
                    begin, end, data.begin(), init, binary_op);
            });
    };

    BENCHMARK_ADVANCED("ana_ex_seq_tiled")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure(
            [begin, end, &binary_op, &data, init]() {
                sequential::tiled::exclusive_scan(
                    begin, end, data.begin(), init, binary_op);
            });
    };
}
SCENARIO("Analytical Exclusive Scan OpenMP", "[ex] [omp]")
{

    // Benchmark parameters
    const size_t N = GENERATE(logRange(1ull << 15, 1ull << 30, 2));

    // Logging of variables
    CAPTURE(N);
    SUCCEED();

    std::vector<bool> data(N);
    std::fill(data.begin(), data.end(), false);

    pad::transform_iterator begin(0, [](ssize_t idx) { return (idx % 2) == true; });
    auto                    end = begin + N;

    auto binary_op = [](bool x, bool y) -> bool { return x ^ y; };
    bool init      = false;

    BENCHMARK_ADVANCED("ana_ex_OMP_provided")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure(
            [begin, end, &binary_op, &data, init]()
            { openmp::provided::exclusive_scan(begin, end, data.begin(), init); });
    };

    BENCHMARK_ADVANCED("ana_ex_OMP_updown")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure(
            [begin, end, &binary_op, &data, init]()
            { openmp::updown::exclusive_scan(begin, end, data.begin(), init); });
    };

    BENCHMARK_ADVANCED("ana_ex_OMP_tiled")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure([begin, end, &binary_op, &data, init]()
                      { openmp::tiled::exclusive_scan(begin, end, data.begin(), init); });
    };
}

SCENARIO("Analytical Exclusive Scan TBB", "[ex] [tbb]")
{

    // Benchmark parameters
    const size_t N = GENERATE(logRange(1ull << 15, 1ull << 30, 2));

    // Logging of variables
    CAPTURE(N);
    SUCCEED();

    std::vector<bool> data(N);
    std::fill(data.begin(), data.end(), false);

    pad::transform_iterator begin(0, [](ssize_t idx) { return (idx % 2) == true; });
    auto                    end = begin + N;

    auto binary_op = [](bool x, bool y) -> bool { return x ^ y; };
    bool init      = false;

    BENCHMARK_ADVANCED("ana_ex_TBB_provided")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure(
            [begin, end, &binary_op, &data, init]()
            { _tbb::provided::exclusive_scan(begin, end, data.begin(), init); });
    };

    BENCHMARK_ADVANCED("ana_ex_TBB_updown")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure(
            [begin, end, &binary_op, &data, init]()
            { _tbb::updown::exclusive_scan(begin, end, data.begin(), init, binary_op); });
    };

    BENCHMARK_ADVANCED("ana_ex_TBB_tiled")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure(
            [begin, end, &binary_op, &data, init]()
            { _tbb::tiled::exclusive_scan(begin, end, data.begin(), init, binary_op); });
    };
}

SCENARIO("Analytical Inclusive Segmented Scan Sequential", "[inc] [seg] [seq]")
{

    // Benchmark parameters
    const size_t N = GENERATE(logRange(1ull << 15, 1ull << 30, 2));

    // Logging of variables
    CAPTURE(N);
    SUCCEED();
    std::vector<std::pair<bool, bool>> data(N);
    std::fill(data.begin(), data.end(), std::make_pair(false, false));

    pad::transform_iterator begin(0,
                                  [](ssize_t idx)
                                  {
                                      if ((idx % 2) == true)
                                      {
                                          return std::make_pair(true, true);
                                      }
                                      else
                                      {
                                          return std::make_pair(false, false);
                                      }
                                  });
    auto                    end = begin + N;

    auto binary_op = [](bool x, bool y) -> bool { return x ^ y; };

    // Benchmark
    BENCHMARK_ADVANCED("ana_incseg_seq_sequential")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [begin, end, &binary_op, &data]()
            { sequential::naive::inclusive_segmented_scan(begin, end, data.begin()); });
    };

    BENCHMARK_ADVANCED("ana_incseg_seq_updown")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [begin, end, &binary_op, &data]()
            { sequential::updown::inclusive_segmented_scan(begin, end, data.begin()); });
    };

    BENCHMARK_ADVANCED("ana_incseg_seq_tiled")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [begin, end, &binary_op, &data]()
            { sequential::tiled::inclusive_segmented_scan(begin, end, data.begin()); });
    };
}

SCENARIO("Analytical Inclusive Segmented Scan OpenMP", "[inc] [seg] [omp]")
{

    // Benchmark parameters
    const size_t N = GENERATE(logRange(1ull << 15, 1ull << 30, 2));

    // Logging of variables
    CAPTURE(N);
    SUCCEED();
    std::vector<std::pair<bool, bool>> data(N);
    std::fill(data.begin(), data.end(), std::make_pair(false, false));

    pad::transform_iterator begin(0,
                                  [](ssize_t idx)
                                  {
                                      if ((idx % 2) == true)
                                      {
                                          return std::make_pair(true, true);
                                      }
                                      else
                                      {
                                          return std::make_pair(false, false);
                                      }
                                  });
    auto                    end = begin + N;

    auto binary_op = [](bool x, bool y) -> bool { return x ^ y; };

    BENCHMARK_ADVANCED("ana_incseg_OMP_updown")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [begin, end, &binary_op, &data]()
            { openmp::updown::inclusive_segmented_scan(begin, end, data.begin()); });
    };

    BENCHMARK_ADVANCED("ana_incseg_OMP_tiled")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [begin, end, &binary_op, &data]()
            { openmp::tiled::inclusive_segmented_scan(begin, end, data.begin()); });
    };
}
SCENARIO("Analytical Inclusive Segmented Scan TBB", "[inc] [seg] [tbb]")
{

    // Benchmark parameters
    const size_t N = GENERATE(logRange(1ull << 15, 1ull << 30, 2));

    // Logging of variables
    CAPTURE(N);
    SUCCEED();
    std::vector<std::pair<bool, bool>> data(N);
    std::fill(data.begin(), data.end(), std::make_pair(false, false));

    pad::transform_iterator begin(0,
                                  [](ssize_t idx)
                                  {
                                      if ((idx % 2) == true)
                                      {
                                          return std::make_pair(true, true);
                                      }
                                      else
                                      {
                                          return std::make_pair(false, false);
                                      }
                                  });
    auto                    end = begin + N;

    auto binary_op = [](bool x, bool y) -> bool { return x ^ y; };

    BENCHMARK_ADVANCED("ana_incseg_TBB_provided")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [begin, end, &binary_op, &data]() {
                _tbb::provided::inclusive_segmented_scan(begin, end, data.begin(), false);
            });
    };

    BENCHMARK_ADVANCED("ana_incseg_TBB_updown")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [begin, end, &binary_op, &data]()
            { _tbb::updown::inclusive_segmented_scan(begin, end, data.begin()); });
    };

    BENCHMARK_ADVANCED("ana_incseg_TBB_tiled")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [begin, end, &binary_op, &data]()
            { _tbb::tiled::inclusive_segmented_scan(begin, end, data.begin()); });
    };
}
/*

SCENARIO("Analytical Exclusive Segmented Scan Sequential", "[ex] [seg] [seq]")
{

    // Benchmark parameters
    const size_t N = GENERATE(logRange(1ull << 15, 1ull << 30, 2));

    // Logging of variables
    CAPTURE(N);
    SUCCEED();

    std::vector<std::pair<float, int>> data(N);
    std::generate(begin,
                  end,
                  [&rand, &flag_rand]()
                  {
                      std::pair<float, int> A;
                      A.first  = rand();
                      A.second = flag_rand();
                      return A;
                  });

    bool init = false;

    // Benchmark
    BENCHMARK_ADVANCED("ana_exseg_seq_sequential")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [begin, end, &binary_op, &data, init]()
            { sequential::naive::exclusive_segmented_scan(data.begin(), end, init); });
    };

    BENCHMARK_ADVANCED("ana_exseg_seq_updown")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [begin, end, &binary_op, &data, init]() {
                sequential::updown::exclusive_segmented_scan(
                    data.begin(), end, .0f, init);
            });
    };

    BENCHMARK_ADVANCED("ana_exseg_seq_tiled")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [begin, end, &binary_op, &data, init]() {
                sequential::tiled::exclusive_segmented_scan(data.begin(), end, .0f, init);
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
    std::generate(begin,
                  end,
                  [&rand, &flag_rand]()
                  {
                      std::pair<float, int> A;
                      A.first  = rand();
                      A.second = flag_rand();
                      return A;
                  });

    bool init = false;

    BENCHMARK_ADVANCED("ana_exseg_OMP_updown")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [begin, end, &binary_op, &data, init]()
            { openmp::updown::exclusive_segmented_scan(data.begin(), end, .0f, init); });
    };

    BENCHMARK_ADVANCED("ana_exseg_OMP_tiled")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure(
            [begin, end, &binary_op, &data, init]()
            { openmp::tiled::exclusive_segmented_scan(data.begin(), end, .0f, init); });
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
    std::generate(begin,
                  end,
                  [&rand, &flag_rand]()
                  {
                      std::pair<float, int> A;
                      A.first  = rand();
                      A.second = flag_rand();
                      return A;
                  });

    bool init = false;

    BENCHMARK_ADVANCED("ana_exseg_TBB_provided")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure([begin, end, &binary_op, &data, init]()
                      { _tbb::provided::exclusive_segmented_scan(begin, end, data.begin()
init); });
    };

    BENCHMARK_ADVANCED("ana_exseg_TBB_updown")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure([begin, end, &binary_op, &data, init]()
                      { _tbb::updown::exclusive_segmented_scan(begin, end, data.begin()
init); });
    };

    BENCHMARK_ADVANCED("ana_exseg_TBB_tiled")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<std::pair<float, int>> result(N);
        meter.measure([begin, end, &binary_op, &data, init]()
                      { _tbb::tiled::exclusive_segmented_scan(begin, end, data.begin()
init); });
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
    std::generate(begin, end, data.begin() rand);

    // Benchmark
    BENCHMARK_ADVANCED("ana_omp_outofplace_inc_tilesize")
    (Catch::Benchmark::Chronometer meter)
    {
        std::vector<float> result(N);
        openmp::tiled::set_tile_size(tile_size);
        meter.measure([begin, end, &binary_op, &data]()
                      { openmp::tiled::inclusive_scan(begin, end,data.begin()); });
    };

    BENCHMARK_ADVANCED("ana_omp_inplace_inc_tilesize")
    (Catch::Benchmark::Chronometer meter)
    {
        openmp::tiled::set_tile_size(tile_size);
        meter.measure([begin, end, &binary_op, &data]()
                      { openmp::tiled::inclusive_scan(begin, end, data.begin()); });
    };
}
*/
