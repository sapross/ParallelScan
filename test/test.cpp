#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_FAST_COMPILE
#include <catch2/catch.hpp>

#include "scan.hpp"

TEST_CASE("Test")
{
    // Test parameters
    size_t N = GENERATE(1, 2, 3, 8, 13, 16, 23, 32);
    using namespace dissemination_Barrier;
    
    // Logging of parameters
    CAPTURE(N);

    
    //Tests
    REQUIRE(1==1);
    CHECK(true==true);
}

