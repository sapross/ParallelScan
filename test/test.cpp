#define CATCH_CONFIG_FAST_COMPILE
#include <catch2/catch.hpp>

#include <algorithm>
#include <numeric>
#include <random>

#include "scan.hpp"

std::default_random_engine         generator;
std::uniform_int_distribution<int> distribution(1, 10);
auto                               randnum = std::bind(distribution, generator);

TEST_CASE("Inclusive Scan Test")
{
    // Test parameters
    size_t N = GENERATE(1, 10, 100);
    // Logging of parameters
    CAPTURE(N);

    std::vector<int> data(N, 0);
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
    // SECTION("Naive Up-Down-Sweep") { ; }
    // SECTION("Naive Tiled") { ; }
}

TEST_CASE("Exclusive Scan Test")
{
    // Test parameters
    size_t N = GENERATE(1, 10, 100);
    // Logging of parameters
    CAPTURE(N);

    std::vector<int> data(N, 0);
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
    // SECTION("Naive Up-Down-Sweep") { ; }
    // SECTION("Naive Tiled") { ; }
}
