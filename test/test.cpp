#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_FAST_COMPILE
#include "logrange_generator"
#include <catch2/catch.hpp>

#include <algortihm>
#include <numeric>
#include <random>

#include "scan.hpp"

std::default_random_engine         generator;
std::uniform_int_distribution<int> distribution(1, 10);
auto                               rand = std::bind(distribution, generator);

TEST_CASE("Inclusive Scan Test")
{
    // Test parameters
    size_t N = LogRange(1, 100, 10);
    // Logging of parameters
    CAPTURE(N);

    std::vector<int> data(N, 0);
    std::for_each(data.begin(), data.end(), [=](auto& element) { element = rand(); });

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
