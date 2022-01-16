#define CATCH_CONFIG_FAST_COMPILE
#include "logrange_generator.hpp"
#include <algorithm>
#include <catch2/catch.hpp>
#include <numeric>
#include <random>

#include "scan.hpp"

std::default_random_engine         generator;
std::uniform_int_distribution<int> distribution(1, 10);
auto                               randnum = std::bind(distribution, generator);

TEST_CASE("Inclusive Scan Test", "[inc]")
{
    // Test parameters
    size_t N = GENERATE(logRange(2, 1ull << 5, 2));
    // Logging of parameters
    CAPTURE(N);

    std::vector<int> data(N, 1);
    std::generate(data.begin(), data.end(), randnum);

    std::vector<int> reference(N, 0);
    std::inclusive_scan(data.begin(), data.end(), reference.begin());

    // Tests
    SECTION("Naive Sequential")
    {
        std::vector<int> result(N, 0);
        sequential::naive::inclusive_scan(data.begin(), data.end(), result.begin());
        REQUIRE_THAT(result, Catch::Matchers::Equals(reference));
    }
    SECTION("Sequential Up-Down-Sweep")
    {
        std::vector<int> result(N, 0);
        sequential::updown::inclusive_scan(data.begin(), data.end(), result.begin());
        REQUIRE_THAT(result, Catch::Matchers::Equals(reference));
    }
    SECTION("Sequential Tiled")
    {
        std::vector<int> result(N, 0);
        sequential::tiled::inclusive_scan(data.begin(), data.end(), result.begin());
        REQUIRE_THAT(result, Catch::Matchers::Equals(reference));
    }
    SECTION("TBB provided")
    {
        std::vector<int> result(N, 0);
        _tbb::provided::inclusive_scan(data.begin(), data.end(), 0, result.begin());
        REQUIRE_THAT(result, Catch::Matchers::Equals(reference));
    }
    SECTION("TBB Up-Down Sweep")
    {
        std::vector<int> result(N, 0);
        _tbb::updown::inclusive_scan(data.begin(), data.end(), result.begin());
        REQUIRE_THAT(result, Catch::Matchers::Equals(reference));
    }
    SECTION("TBB Tiled")
    {
        std::vector<int> result(N, 0);
        _tbb::tiled::inclusive_scan(data.begin(), data.end(), result.begin());
        REQUIRE_THAT(result, Catch::Matchers::Equals(reference));
    }
    SECTION("OpenMP provided")
    {
        std::vector<int> result(N, 0);
        openmp::provided::inclusive_scan(data.begin(), data.end(), result.begin());
        REQUIRE_THAT(result, Catch::Matchers::Equals(reference));
    }
    SECTION("OpenMP Up-Down Sweep")
    {
        std::vector<int> result(N, 0);
        openmp::updown::inclusive_scan(data.begin(), data.end(), result.begin());
        REQUIRE_THAT(result, Catch::Matchers::Equals(reference));
    }
    SECTION("OpenMP Tiled")
    {
        std::vector<int> result(N, 0);
        openmp::tiled::inclusive_scan(data.begin(), data.end(), result.begin());
        REQUIRE_THAT(result, Catch::Matchers::Equals(reference));
    }
    // SECTION("Sequential Up-Down-Sweep") { ; }
    // SECTION("Sequential Tiled") { ; }
}

TEST_CASE("Exclusive Scan Test", "[ex]")
{
    // Test parameters
    size_t N = GENERATE(logRange(2, 1ull << 5, 2));
    // Logging of parameters
    CAPTURE(N);

    std::vector<int> data(N, 1);
    std::generate(data.begin(), data.end(), randnum);

    std::vector<int> reference(N, 0);
    std::exclusive_scan(data.begin(), data.end(), reference.begin(), 0);

    // Tests
    SECTION("Sequential Sequential")
    {
        std::vector<int> result(N, 0);
        sequential::naive::exclusive_scan(data.begin(), data.end(), result.begin(), 0);
        REQUIRE_THAT(result, Catch::Matchers::Equals(reference));
    }
    SECTION("Sequential Up-Down-Sweep")
    {
        std::vector<int> result(N, 0);
        sequential::updown::exclusive_scan(data.begin(), data.end(), result.begin(), 0);
        REQUIRE_THAT(result, Catch::Matchers::Equals(reference));
    }
    SECTION("Sequential Up-Down-Sweep Tiled")
    {
        std::vector<int> result(N, 0);
        sequential::tiled::exclusive_scan(data.begin(), data.end(), result.begin(), 0);
        REQUIRE_THAT(result, Catch::Matchers::Equals(reference));
    }
    SECTION("TBB provided")
    {
        std::vector<int> result(N, 0);
        _tbb::provided::exclusive_scan(data.begin(), data.end(), result.begin(), 0);
        REQUIRE_THAT(result, Catch::Matchers::Equals(reference));
    }
    SECTION("TBB Up-Down Sweep")
    {
        std::vector<int> result(N, 0);
        _tbb::updown::exclusive_scan(data.begin(), data.end(), result.begin(), 0);
        REQUIRE_THAT(result, Catch::Matchers::Equals(reference));
    }
    SECTION("TBB Tiled")
    {
        std::vector<int> result(N, 0);
        _tbb::tiled::exclusive_scan(data.begin(), data.end(), result.begin(), 0);
        REQUIRE_THAT(result, Catch::Matchers::Equals(reference));
    }
    SECTION("OpenMP provided")
    {
        std::vector<int> result(N, 0);
        openmp::provided::exclusive_scan(data.begin(), data.end(), result.begin(), 0);
        REQUIRE_THAT(result, Catch::Matchers::Equals(reference));
    }
    SECTION("OpenMP Up-Down-Sweep")
    {
        std::vector<int> result(N, 0);
        openmp::updown::exclusive_scan(data.begin(), data.end(), result.begin(), 0);
        REQUIRE_THAT(result, Catch::Matchers::Equals(reference));
    }
    SECTION("OpenMP Up-Down-Sweep Tiled")
    {
        std::vector<int> result(N, 0);
        openmp::tiled::exclusive_scan(data.begin(), data.end(), result.begin(), 0);
        REQUIRE_THAT(result, Catch::Matchers::Equals(reference));
    }

    // SECTION("Sequential Up-Down-Sweep") { ; }
    // SECTION("Sequential Tiled") { ; }
}

TEST_CASE("Inclusive Segmented Scan Test", "[incseg]")
{
    // Test parameters
    size_t N = 16;
    // Logging of parameters
    CAPTURE(N);

    std::vector<int> values(
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16}); //, 17, 18, 19,
                                                                  // 20});
    std::vector<int> flags(
        {0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0}); //, 1, 0, 0, 1});
    std::vector<std::pair<int, int>> data(N);
    for (size_t i = 0; i < N; i++)
    {
        data[i] = std::make_pair(values[i], flags[i]);
    }

    std::vector<int> reference(
        {1, 3, 3, 7, 5, 11, 7, 15, 24, 34, 45, 12, 13, 14, 29, 45}); //, 17, 35, 54,
                                                                     // 20});
    // Tests
    SECTION("Sequential Sequential")
    {
        std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));
        sequential::naive::inclusive_segmented_scan(
            data.begin(), data.end(), result.begin());

        std::vector<int> temp(N);
        for (size_t i = 0; i < N; i++)
        {
            temp[i] = result[i].first;
        }
        REQUIRE_THAT(temp, Catch::Matchers::Equals(reference));
    }
    SECTION("Sequential Up-Down-Sweep")
    {
        std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));
        sequential::updown::inclusive_segmented_scan(
            data.begin(), data.end(), result.begin());

        std::vector<int> temp(N);
        for (size_t i = 0; i < N; i++)
        {
            temp[i] = result[i].first;
        }
        REQUIRE_THAT(temp, Catch::Matchers::Equals(reference));
    }
    SECTION("Sequential Tiled")
    {
        std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));
        sequential::tiled::inclusive_segmented_scan(
            data.begin(), data.end(), result.begin());

        std::vector<int> temp(N);
        for (size_t i = 0; i < N; i++)
        {
            temp[i] = result[i].first;
        }
        REQUIRE_THAT(temp, Catch::Matchers::Equals(reference));
    }
    SECTION("TBB provided")
    {
        std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));
        _tbb::provided::inclusive_segmented_scan(
            data.begin(), data.end(), result.begin(), 0);

        std::vector<int> temp(N);
        for (size_t i = 0; i < N; i++)
        {
            temp[i] = result[i].first;
        }
        REQUIRE_THAT(temp, Catch::Matchers::Equals(reference));
    }
    SECTION("TBB Up-Down Sweep")
    {
        std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));
        _tbb::updown::inclusive_segmented_scan(data.begin(), data.end(), result.begin());
        std::vector<int> temp(N);
        for (size_t i = 0; i < N; i++)
        {
            temp[i] = result[i].first;
        }
        REQUIRE_THAT(temp, Catch::Matchers::Equals(reference));
    }
    SECTION("TBB Tiled")
    {
        std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));
        _tbb::tiled::inclusive_segmented_scan(data.begin(), data.end(), result.begin());
        std::vector<int> temp(N);
        for (size_t i = 0; i < N; i++)
        {
            temp[i] = result[i].first;
        }
        REQUIRE_THAT(temp, Catch::Matchers::Equals(reference));
    }
    // Does not work with provided scan functionality
    // SECTION("OpenMP provided")
    //{
    // std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));
    // openmp::provided::inclusive_segmented_scan(
    //  data.begin(), data.end(), result.begin());

    // std::vector<int> temp(N);
    // for (size_t i = 0; i < N; i++)
    //{
    //  temp[i] = result[i].first;
    //}
    //  REQUIRE_THAT(temp, Catch::Matchers::Equals(reference));
    //}
    SECTION("OpenMP Up-Down-Sweep")
    {
        std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));
        openmp::updown::inclusive_segmented_scan(
            data.begin(), data.end(), result.begin());

        std::vector<int> temp(N);
        for (size_t i = 0; i < N; i++)
        {
            temp[i] = result[i].first;
        }
        REQUIRE_THAT(temp, Catch::Matchers::Equals(reference));
    }
    SECTION("OpenMP Tiled")
    {
        std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));
        openmp::tiled::inclusive_segmented_scan(data.begin(), data.end(), result.begin());

        std::vector<int> temp(N);
        for (size_t i = 0; i < N; i++)
        {
            temp[i] = result[i].first;
        }
        REQUIRE_THAT(temp, Catch::Matchers::Equals(reference));
    }

    // SECTION("Sequential Up-Down-Sweep") { ; }
    // SECTION("Sequential Tiled") { ; }
}

TEST_CASE("Exclusive Segmented Scan Test", "[exseg]")
{
    // Test parameters
    size_t N = 16;
    // Logging of parameters
    CAPTURE(N);

    std::vector<int> values(
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16}); //, 17, 18, 19,
                                                                  // 20});
    std::vector<int> flags(
        {0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0}); //, 1, 0, 0, 0});
    std::vector<std::pair<int, int>> data(N);
    for (size_t i = 0; i < N; i++)
    {
        data[i] = std::make_pair(values[i], flags[i]);
    }

    std::vector<int> reference(
        {0, 1, 0, 3, 0, 5, 0, 7, 15, 24, 34, 0, 0, 0, 14, 29}); //, 0, 17, 35, 0});

    // Tests
    SECTION("Sequential Sequential")
    {
        std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));

        sequential::naive::exclusive_segmented_scan(
            data.begin(), data.end(), result.begin(), 0);
        std::vector<int> temp(N);
        for (size_t i = 0; i < N; i++)
        {
            temp[i] = result[i].first;
        }
        REQUIRE_THAT(temp, Catch::Matchers::Equals(reference));
    }
    SECTION("Sequential Up-Down-Sweep")
    {
        std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));

        sequential::updown::exclusive_segmented_scan(
            data.begin(), data.end(), result.begin(), 0);
        std::vector<int> temp(N);
        for (size_t i = 0; i < N; i++)
        {
            temp[i] = result[i].first;
        }
        REQUIRE_THAT(temp, Catch::Matchers::Equals(reference));
    }
    SECTION("Sequential Tiled")
    {
        std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));

        sequential::tiled::exclusive_segmented_scan(
            data.begin(), data.end(), result.begin(), 0);
        std::vector<int> temp(N);
        for (size_t i = 0; i < N; i++)
        {
            temp[i] = result[i].first;
        }
        REQUIRE_THAT(temp, Catch::Matchers::Equals(reference));
    }
    // Does not work with provided scan functionality
    // SECTION("OpenMP provided")
    //{
    //  std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));
    // openmp::provided::exclusive_segmented_scan(
    //  data.begin(), data.end(), result.begin());

    // std::vector<int> temp(N);
    // for (size_t i = 0; i < N; i++)
    //{
    //   temp[i] = result[i].first;
    //}
    // REQUIRE_THAT(temp, Catch::Matchers::Equals(reference));
    //}
    SECTION("OpenMP Up-Down-Sweep")
    {
        std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));

        openmp::updown::exclusive_segmented_scan(
            data.begin(), data.end(), result.begin(), 0);
        std::vector<int> temp(N);
        for (size_t i = 0; i < N; i++)
        {
            temp[i] = result[i].first;
        }
        REQUIRE_THAT(temp, Catch::Matchers::Equals(reference));
    }
    SECTION("OpenMP Tiled")
    {
        std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));

        openmp::tiled::exclusive_segmented_scan(
            data.begin(), data.end(), result.begin(), 0);
        std::vector<int> temp(N);
        for (size_t i = 0; i < N; i++)
        {
            temp[i] = result[i].first;
        }
        REQUIRE_THAT(temp, Catch::Matchers::Equals(reference));
    }
    SECTION("TBB provided")
    {
        std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));
        _tbb::provided::exclusive_segmented_scan(
            data.begin(), data.end(), result.begin(), 0);
        std::vector<int> temp(N);
        for (size_t i = 0; i < N; i++)
        {
            temp[i] = result[i].first;
        }
        REQUIRE_THAT(temp, Catch::Matchers::Equals(reference));
    }
    SECTION("TBB Up-Down Sweep")
    {
        std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));
        _tbb::updown::exclusive_segmented_scan(
            data.begin(), data.end(), result.begin(), 0);

        std::vector<int> temp(N);
        for (size_t i = 0; i < N; i++)
        {
            temp[i] = result[i].first;
        }
        REQUIRE_THAT(temp, Catch::Matchers::Equals(reference));
    }
    SECTION("TBB Tiled")
    {
        std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));
        _tbb::tiled::exclusive_segmented_scan(
            data.begin(), data.end(), result.begin(), 0);

        std::vector<int> temp(N);
        for (size_t i = 0; i < N; i++)
        {
            temp[i] = result[i].first;
        }
        REQUIRE_THAT(temp, Catch::Matchers::Equals(reference));
    }
}
