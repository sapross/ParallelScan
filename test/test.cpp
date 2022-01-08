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

TEST_CASE("Inclusive Scan Test")
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
        naive::sequential::inclusive_scan(data.begin(), data.end(), result.begin());
        REQUIRE_THAT(result, Catch::Matchers::Equals(reference));
    }
    SECTION("Naive Up-Down-Sweep")
    {
        std::vector<int> result(N, 0);
        naive::updown::inclusive_scan(data.begin(), data.end(), result.begin());
        REQUIRE_THAT(result, Catch::Matchers::Equals(reference));
    }
    SECTION("Naive Tiled")
    {
        std::vector<int> result(N, 0);
        naive::tiled::inclusive_scan(data.begin(), data.end(), result.begin());
        REQUIRE_THAT(result, Catch::Matchers::Equals(reference));
    }
    SECTION("TBB")
    {
        std::vector<int> result(N,0);
        _tbb::inclusive_scan(data.begin(), data.end(), result.begin());
        REQUIRE_THAT(result, Catch::Matchers::Equals(reference));
    }
    // SECTION("Naive Up-Down-Sweep") { ; }
    // SECTION("Naive Tiled") { ; }
}

TEST_CASE("Exclusive Scan Test")
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
    SECTION("Naive Sequential")
    {
        std::vector<int> result(N, 0);
        naive::sequential::exclusive_scan(data.begin(), data.end(), result.begin(), 0);
        REQUIRE_THAT(result, Catch::Matchers::Equals(reference));
    }
    SECTION("Naive Up-Down-Sweep")
    {
        std::vector<int> result(N, 0);
        naive::updown::exclusive_scan(data.begin(), data.end(), result.begin(), 0);
        REQUIRE_THAT(result, Catch::Matchers::Equals(reference));
    }
    SECTION("Naive Up-Down-Sweep")
    {
        std::vector<int> result(N, 0);
        naive::tiled::exclusive_scan(data.begin(), data.end(), result.begin(), 0);
        REQUIRE_THAT(result, Catch::Matchers::Equals(reference));
    }
    SECTION("TBB")
    {
        std::vector<int> result(0,N+1);
        _tbb::exclusive_scan(data.begin(), data.end(), result.begin(), 0);
        
        std::cout << "data: ";
        std::for_each(data.begin(), data.end(), [](auto e) { std::cout << e << ", "; });
        std::cout << std::endl;
        std::cout << "result: ";
        std::for_each(result.begin(), result.end(), [](auto e) { std::cout << e << ", "; });
        std::cout << std::endl;
        
        REQUIRE_THAT(result, Catch::Matchers::Equals(reference));
    }
    // SECTION("Naive Up-Down-Sweep") { ; }
    // SECTION("Naive Tiled") { ; }
}

TEST_CASE("Inclusive Segmented Scan Test")
{
    // Test parameters
    size_t N = 20;
    // Logging of parameters
    CAPTURE(N);

    std::vector<int> data(
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20});
    std::vector<int> flags({0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1});
    std::vector<int> reference(
        {1, 3, 3, 7, 5, 11, 7, 15, 24, 34, 45, 12, 13, 14, 29, 45, 17, 35, 54, 20});
    // Tests
    SECTION("Naive Sequential")
    {
        std::vector<int> result(N, 0);
        naive::sequential::inclusive_segmented_scan(
            data.begin(), data.end(), flags.begin(), result.begin());
        REQUIRE_THAT(result, Catch::Matchers::Equals(reference));
    }
    // SECTION("Naive Up-Down-Sweep") { ; }
    // SECTION("Naive Tiled") { ; }
}

TEST_CASE("Exclusive Segmented Scan Test")
{
    // Test parameters
    size_t N = 20;
    // Logging of parameters
    CAPTURE(N);

    std::vector<int> data(
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20});
    std::vector<int> flags(
        {0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0});
    std::vector<int> reference(
        {0, 1, 0, 3, 0, 5, 0, 7, 15, 24, 34, 0, 0, 0, 14, 29, 0, 17, 35, 0});

    // Tests
    SECTION("Naive Sequential")
    {
        std::vector<int> result(N, 0);
        naive::sequential::exclusive_segmented_scan(
            data.begin(), data.end(), flags.begin(), result.begin(), 0);
        REQUIRE_THAT(result, Catch::Matchers::Equals(reference));
    }
    // SECTION("Naive Up-Down-Sweep") { ; }
    // SECTION("Naive Tiled") { ; }
}
