#define CATCH_CONFIG_FAST_COMPILE
#include "logrange_generator.hpp"
#include <algorithm>
#include <catch2/catch.hpp>
#include <iomanip>
#include <numeric>
#include <random>
#include <sstream>

#include "scan.hpp"

// The matcher class
class PairVectorFirstEquals: public Catch::MatcherBase<std::vector<std::pair<int, int>>>
{
    std::vector<std::pair<int, int>> const& ref;

  public:
    PairVectorFirstEquals(std::vector<std::pair<int, int>> const& _ref): ref(_ref) {}

    // Performs the test for this matcher
    bool match(std::vector<std::pair<int, int>> const& data) const
    {
        if (ref.size() != data.size())
        {
            return false;
        }
        for (size_t i = 0; i < ref.size(); i++)
        {
            if (data[i].first != ref[i].first)
            {
                return false;
            }
        }
        return true;
    }

    // Produces a string describing what this matcher does. It should
    // include any provided data (the begin/ end in this case) and
    // be written as if it were stating a fact (in the output it will be
    // preceded by the value under test).
    std::string describe() const
    {
        std::ostringstream ss;
        auto               it = ref.begin();

        ss << "First Pairs Equals { {" << (*it).first << "}";
        while (++it != ref.end())
        {
            ss << ", {" << (*it).first << "}";
        }
        ss << " }";
        return ss.str();
    }
};

// The builder function
inline PairVectorFirstEquals
PairsFirstsEqual(std::vector<std::pair<int, int>> const& _ref)
{
    return PairVectorFirstEquals(_ref);
}

//----------------------------------------------------------------------
// Out-Of-Place Tests
//----------------------------------------------------------------------

TEST_CASE("Out-Of-Place Inclusive Scan Test", "[out][inc]")
{
    // Test parameters
    size_t N = GENERATE(logRange(2, 1ull << 5, 2));
    // Logging of parameters
    CAPTURE(N);

    std::default_random_engine         generator;
    std::uniform_int_distribution<int> distribution(1, 10);
    auto                               randnum = std::bind(distribution, generator);

    std::vector<int> data(N, 1);
    std::generate(data.begin(), data.end(), randnum);

    std::vector<int> reference(N, 0);
    std::inclusive_scan(data.begin(), data.end(), reference.begin());

    //Tests
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
}

TEST_CASE("Out-Of-Place Exclusive Scan Test", "[out][ex]")
{
    // Test parameters
    size_t N = GENERATE(logRange(2, 1ull << 5, 2));
    // Logging of parameters
    CAPTURE(N);

    std::default_random_engine         generator;
    std::uniform_int_distribution<int> distribution(1, 10);
    auto                               randnum = std::bind(distribution, generator);

    std::vector<int> data(N, 1);
    std::generate(data.begin(), data.end(), randnum);

    std::vector<int> reference(N, 0);
    std::exclusive_scan(data.begin(), data.end(), reference.begin(), 0);

    // // Tests
    SECTION("Sequential Naive")
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
}

TEST_CASE("Out-Of-Place Inclusive Segmented Scan Sequential Test", "[out][incseg][seq]")
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
    SECTION("Sequential Naive")
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
}

TEST_CASE("Out-Of-Place Inclusive Segmented Scan Test", "[out][incseg]")
{

    // Benchmark parameters
    const size_t N = GENERATE(logRange(1ull << 4, 1ull << 10, 2));

    // Logging of variables
    CAPTURE(N);
    SUCCEED();

    std::default_random_engine         generator;
    std::uniform_int_distribution<int> distribution(1, 10);
    auto                               randnum = std::bind(distribution, generator);

    std::default_random_engine         flag_generator;
    std::uniform_int_distribution<int> flag_distribution(0, 1);
    auto flag_rand = std::bind(flag_distribution, flag_generator);

    std::vector<std::pair<int, int>> data(N);
    std::generate(data.begin(),
                  data.end(),
                  [&randnum, &flag_rand]()
                  {
                      std::pair<int, int> A;
                      A.first  = randnum();
                      A.second = flag_rand();
                      return A;
                  });

    std::vector<std::pair<int, int>> reference(N);
    sequential::naive::inclusive_segmented_scan(
        data.begin(), data.end(), reference.begin());

    SECTION("Sequential Up-Down-Sweep")
    {
        std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));
        sequential::updown::inclusive_segmented_scan(
            data.begin(), data.end(), result.begin());
        REQUIRE_THAT(result, PairsFirstsEqual(reference));
    }
    SECTION("Sequential Tiled")
    {
        std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));
        sequential::tiled::inclusive_segmented_scan(
            data.begin(), data.end(), result.begin());
        REQUIRE_THAT(result, PairsFirstsEqual(reference));
    }
    SECTION("TBB provided")
    {
        std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));
        _tbb::provided::inclusive_segmented_scan(
            data.begin(), data.end(), result.begin(), 0);
        REQUIRE_THAT(result, PairsFirstsEqual(reference));
    }
    SECTION("TBB Up-Down Sweep")
    {
        std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));
        _tbb::updown::inclusive_segmented_scan(data.begin(), data.end(), result.begin());
        REQUIRE_THAT(result, PairsFirstsEqual(reference));
    }
    SECTION("TBB Tiled")
    {
        std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));
        _tbb::tiled::inclusive_segmented_scan(data.begin(), data.end(), result.begin());
        REQUIRE_THAT(result, PairsFirstsEqual(reference));
    }
    SECTION("OpenMP Up-Down-Sweep")
    {
        std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));
        openmp::updown::inclusive_segmented_scan(
            data.begin(), data.end(), result.begin());
        REQUIRE_THAT(result, PairsFirstsEqual(reference));
    }
    SECTION("OpenMP Tiled")
    {
        std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));
        // std::cout << "OpenMP updown" << std::endl;
        // std::cout << "Input" << std::endl;
        // std::for_each(data.begin(),
        //               data.end(),
        //               [](auto x) { std::cout << std::setw(2) << x.first << ", "; });
        // std::cout << std::endl;
        // std::for_each(data.begin(),
        //               data.end(),
        //               [](auto x) { std::cout << std::setw(2) << x.second << ", "; });
        // std::cout << std::endl;

        // std::cout << "Reference" << std::endl;
        // std::for_each(reference.begin(),
        //               reference.end(),
        //               [](auto x) { std::cout << std::setw(2) << x.first << ", "; });
        // std::cout << std::endl;

        openmp::tiled::inclusive_segmented_scan(data.begin(), data.end(), result.begin());
        // std::cout << "Output" << std::endl;
        // std::for_each(result.begin(),
        //               result.end(),
        //               [](auto x) { std::cout << std::setw(2) << x.first << ", "; });
        // std::cout << std::endl;
        // std::for_each(result.begin(),
        //               result.end(),
        //               [](auto x) { std::cout << std::setw(2) << x.second << ", "; });
        // std::cout << std::endl;
        REQUIRE_THAT(result, PairsFirstsEqual(reference));
    }
}

TEST_CASE("Out-Of-Place Exclusive Segmented Scan Sequential Test", "[out][exseg][seq]")
{
    // Test parameters
    const size_t N = 16;
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
        {1, 2, 1, 4, 1, 6, 1, 8, 16, 25, 35, 1, 1, 1, 15, 30}); //, 0, 17, 35, 0});

    // Tests
    SECTION("Sequential Naive")
    {
        std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));

        sequential::naive::exclusive_segmented_scan(
            data.begin(), data.end(), result.begin(), 1);
        std::vector<int> temp(N);
        for (size_t i = 0; i < N; i++)
        {
            temp[i] = result[i].first;
        }
        REQUIRE_THAT(temp, Catch::Matchers::Equals(reference));
    }
}

TEST_CASE("Out-Of-Place Exclusive Segmented Scan Test", "[out][exseg]")
{
    // Test parameters
    const size_t N = GENERATE(logRange(1ull << 4, 1ull << 10, 2));

    // Logging of parameters
    CAPTURE(N);

    std::default_random_engine         generator;
    std::uniform_int_distribution<int> distribution(1, 10);
    auto                               randnum = std::bind(distribution, generator);

    std::default_random_engine         flag_generator;
    std::uniform_int_distribution<int> flag_distribution(0, 1);
    auto flag_rand = std::bind(flag_distribution, flag_generator);

    std::vector<std::pair<int, int>> data(N);
    std::generate(data.begin(),
                  data.end(),
                  [&randnum, &flag_rand]()
                  {
                      std::pair<int, int> A;
                      A.first  = randnum();
                      A.second = flag_rand();
                      return A;
                  });

    int init = 5;

    std::vector<std::pair<int, int>> reference(N);
    sequential::naive::exclusive_segmented_scan(
        data.begin(), data.end(), reference.begin(), init);

    SECTION("Sequential Up-Down-Sweep")
    {
        std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));

        sequential::updown::exclusive_segmented_scan(
            data.begin(), data.end(), result.begin(), 0, init);

        REQUIRE_THAT(result, PairsFirstsEqual(reference));
    }
    SECTION("Sequential Tiled")
    {
        std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));
        sequential::tiled::exclusive_segmented_scan(
            data.begin(), data.end(), result.begin(), 0, init);
        REQUIRE_THAT(result, PairsFirstsEqual(reference));
    }
    //Does not work with provided scan functionality
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
            data.begin(), data.end(), result.begin(), 0, init);
        REQUIRE_THAT(result, PairsFirstsEqual(reference));
    }
    SECTION("OpenMP Tiled")
    {
        std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));
        openmp::tiled::exclusive_segmented_scan(
            data.begin(), data.end(), result.begin(), 0, init);
        REQUIRE_THAT(result, PairsFirstsEqual(reference));
    }
    SECTION("TBB provided")
    {
        // std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));
        // _tbb::provided::exclusive_segmented_scan(
        //     data.begin(), data.end(), result.begin(), init);
        // std::cout << "data" << std::endl;
        // std::for_each(data.begin(),
        //               data.end(),
        //               [](auto x) { std::cout << std::setw(2) << x.first << ", "; });
        // std::cout << std::endl;
        // std::cout << "data partitioner" << std::endl;
        // std::for_each(data.begin(),
        //               data.end(),
        //               [](auto x) { std::cout << std::setw(2) << x.second << ", "; });
        // std::cout << std::endl;
        // std::cout << "result" << std::endl;
        // std::for_each(result.begin(),
        //               result.end(),
        //               [](auto x) { std::cout << std::setw(2) << x.first << ", "; });
        // std::cout << std::endl;
        // std::cout << "reference" << std::endl;
        // std::for_each(reference.begin(),
        //               reference.end(),
        //               [](auto x) { std::cout << std::setw(2) << x.first << ", "; });
        // REQUIRE_THAT(result, PairsFirstsEqual(reference));
    }
    SECTION("TBB Up-Down Sweep")
    {
        std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));
        _tbb::updown::exclusive_segmented_scan(
            data.begin(), data.end(), result.begin(), 0, init);
        REQUIRE_THAT(result, PairsFirstsEqual(reference));
    }
    SECTION("TBB Tiled")
    {
        std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));
        _tbb::tiled::exclusive_segmented_scan(
            data.begin(), data.end(), result.begin(), init);
        REQUIRE_THAT(result, PairsFirstsEqual(reference));
    }
}

// //----------------------------------------------------------------------
// // In-Place Tests
// //----------------------------------------------------------------------

TEST_CASE("In-Place Inclusive Scan Test", "[in][inc]")
{
    // Test parameters
    size_t N = GENERATE(logRange(2, 1ull << 5, 2));
    // Logging of parameters
    CAPTURE(N);

    std::default_random_engine         generator;
    std::uniform_int_distribution<int> distribution(1, 10);
    auto                               randnum = std::bind(distribution, generator);

    std::vector<int> data(N, 0);
    std::generate(data.begin(), data.end(), randnum);

    std::vector<int> reference(N, 0);
    std::inclusive_scan(data.begin(), data.end(), reference.begin());

    // Tests
    SECTION("Naive Sequential")
    {
        std::vector<int> result(N, 0);
        std::copy(data.begin(), data.end(), result.begin());
        sequential::naive::inclusive_scan(result.begin(), result.end());
        REQUIRE_THAT(result, Catch::Matchers::Equals(reference));
    }
    SECTION("Sequential Up-Down-Sweep")
    {
        std::vector<int> result(N, 0);
        std::copy(data.begin(), data.end(), result.begin());
        sequential::updown::inclusive_scan(result.begin(), result.end());
        REQUIRE_THAT(result, Catch::Matchers::Equals(reference));
    }
    SECTION("Sequential Tiled")
    {
        std::vector<int> result(N, 0);
        std::copy(data.begin(), data.end(), result.begin());
        sequential::tiled::inclusive_scan(result.begin(), result.end());
        REQUIRE_THAT(result, Catch::Matchers::Equals(reference));
    }
    SECTION("TBB provided")
    {
        std::vector<int> result(N, 0);
        std::copy(data.begin(), data.end(), result.begin());
        _tbb::provided::inclusive_scan(data.begin(), data.end(), 0, result.begin());
        REQUIRE_THAT(result, Catch::Matchers::Equals(reference));
    }
    SECTION("TBB Up-Down Sweep")
    {
        std::vector<int> result(N, 0);
        std::copy(data.begin(), data.end(), result.begin());
        _tbb::updown::inclusive_scan(result.begin(), result.end(), result.begin());
        REQUIRE_THAT(result, Catch::Matchers::Equals(reference));
    }
    SECTION("TBB Tiled")
    {
        std::vector<int> result(N, 0);
        std::copy(data.begin(), data.end(), result.begin());
        _tbb::tiled::inclusive_scan(result.begin(), result.end(), result.begin());
        REQUIRE_THAT(result, Catch::Matchers::Equals(reference));
    }
    SECTION("OpenMP provided")
    {
        std::vector<int> result(N, 0);
        std::copy(data.begin(), data.end(), result.begin());
        openmp::provided::inclusive_scan(result.begin(), result.end(), result.begin());
        REQUIRE_THAT(result, Catch::Matchers::Equals(reference));
    }
    SECTION("OpenMP Up-Down Sweep")
    {
        std::vector<int> result(N, 0);
        std::copy(data.begin(), data.end(), result.begin());
        openmp::updown::inclusive_scan(result.begin(), result.end(), result.begin());
        REQUIRE_THAT(result, Catch::Matchers::Equals(reference));
    }
    SECTION("OpenMP Tiled")
    {
        std::vector<int> result(N, 0);
        std::copy(data.begin(), data.end(), result.begin());
        openmp::tiled::inclusive_scan(result.begin(), result.end(), result.begin());
        REQUIRE_THAT(result, Catch::Matchers::Equals(reference));
    }
}

TEST_CASE("In-Place Exclusive Scan Test", "[in][ex]")
{
    // Test parameters
    size_t N = GENERATE(logRange(2, 1ull << 5, 2));
    // Logging of parameters
    CAPTURE(N);

    std::default_random_engine         generator;
    std::uniform_int_distribution<int> distribution(1, 10);
    auto                               randnum = std::bind(distribution, generator);

    std::vector<int> data(N, 1);
    std::generate(data.begin(), data.end(), randnum);

    std::vector<int> reference(N, 0);
    std::exclusive_scan(data.begin(), data.end(), reference.begin(), 0);

    // Tests
    SECTION("Sequential Naive")
    {
        std::vector<int> result(N, 0);
        std::copy(data.begin(), data.end(), result.begin());
        sequential::naive::exclusive_scan(data.begin(), data.end(), result.begin(), 0);
        REQUIRE_THAT(result, Catch::Matchers::Equals(reference));
    }
    SECTION("Sequential Up-Down-Sweep")
    {
        std::vector<int> result(N, 0);
        std::copy(data.begin(), data.end(), result.begin());
        sequential::updown::exclusive_scan(data.begin(), data.end(), result.begin(), 0);
        REQUIRE_THAT(result, Catch::Matchers::Equals(reference));
    }
    SECTION("Sequential Up-Down-Sweep Tiled")
    {
        std::vector<int> result(N, 0);
        std::copy(data.begin(), data.end(), result.begin());
        sequential::tiled::exclusive_scan(data.begin(), data.end(), result.begin(), 0);
        REQUIRE_THAT(result, Catch::Matchers::Equals(reference));
    }
    SECTION("TBB provided")
    {
        std::vector<int> result(N, 0);
        std::copy(data.begin(), data.end(), result.begin());
        _tbb::provided::exclusive_scan(data.begin(), data.end(), result.begin(), 0);
        REQUIRE_THAT(result, Catch::Matchers::Equals(reference));
    }
    SECTION("TBB Up-Down Sweep")
    {
        std::vector<int> result(N, 0);
        std::copy(data.begin(), data.end(), result.begin());
        _tbb::updown::exclusive_scan(data.begin(), data.end(), result.begin(), 0);
        REQUIRE_THAT(result, Catch::Matchers::Equals(reference));
    }
    SECTION("TBB Tiled")
    {
        std::vector<int> result(N, 0);
        std::copy(data.begin(), data.end(), result.begin());
        _tbb::tiled::exclusive_scan(data.begin(), data.end(), result.begin(), 0);
        REQUIRE_THAT(result, Catch::Matchers::Equals(reference));
    }
//     SECTION("OpenMP provided")
//     {
//         std::vector<int> result(N, 0);
//         std::copy(data.begin(), data.end(), result.begin());
//         openmp::provided::exclusive_scan(data.begin(), data.end(), result.begin(), 0);
//         REQUIRE_THAT(result, Catch::Matchers::Equals(reference));
//     }
//     SECTION("OpenMP Up-Down-Sweep")
//     {
//         std::vector<int> result(N, 0);
//         std::copy(data.begin(), data.end(), result.begin());
//         openmp::updown::exclusive_scan(data.begin(), data.end(), result.begin(), 0);
//         REQUIRE_THAT(result, Catch::Matchers::Equals(reference));
//     }
//     SECTION("OpenMP Up-Down-Sweep Tiled")
//     {
//         std::vector<int> result(N, 0);
//         std::copy(data.begin(), data.end(), result.begin());
//         openmp::tiled::exclusive_scan(data.begin(), data.end(), result.begin(), 0);
//         REQUIRE_THAT(result, Catch::Matchers::Equals(reference));
//     }
}

TEST_CASE("In-Place Inclusive Segmented Scan Sequential Test", "[in][incseg][seq]")
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
    SECTION("Sequential Naive")
    {
        std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));
        std::copy(data.begin(), data.end(), result.begin());
        sequential::naive::inclusive_segmented_scan(result.begin(), result.end());

        std::vector<int> temp(N);
        for (size_t i = 0; i < N; i++)
        {
            temp[i] = result[i].first;
        }
        REQUIRE_THAT(temp, Catch::Matchers::Equals(reference));
    }
}

TEST_CASE("In-Place Inclusive Segmented Scan Test", "[in][incseg]")
{

    // Benchmark parameters
    const size_t N = GENERATE(logRange(1ull << 4, 1ull << 10, 2));

    // Logging of variables
    CAPTURE(N);
    SUCCEED();

    std::default_random_engine         generator;
    std::uniform_int_distribution<int> distribution(1, 10);
    auto                               randnum = std::bind(distribution, generator);

    std::default_random_engine         flag_generator;
    std::uniform_int_distribution<int> flag_distribution(0, 1);
    auto flag_rand = std::bind(flag_distribution, flag_generator);

    std::vector<std::pair<int, int>> data(N);
    std::generate(data.begin(),
                  data.end(),
                  [&randnum, &flag_rand]()
                  {
                      std::pair<int, int> A;
                      A.first  = randnum();
                      A.second = flag_rand();
                      return A;
                  });

    std::vector<std::pair<int, int>> reference(N);
    sequential::naive::inclusive_segmented_scan(
        data.begin(), data.end(), reference.begin());

    SECTION("Sequential Up-Down-Sweep")
    {
        std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));
        std::copy(data.begin(), data.end(), result.begin());
        sequential::updown::inclusive_segmented_scan(result.begin(), result.end());
        REQUIRE_THAT(result, PairsFirstsEqual(reference));
    }
    SECTION("Sequential Tiled")
    {
        std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));
        std::copy(data.begin(), data.end(), result.begin());
        sequential::tiled::inclusive_segmented_scan(result.begin(), result.end());
        REQUIRE_THAT(result, PairsFirstsEqual(reference));
    }
    SECTION("TBB provided")
    {
        std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));
        std::copy(data.begin(), data.end(), result.begin());
        _tbb::provided::inclusive_segmented_scan(
            result.begin(), result.end(), result.begin(), 0);
        REQUIRE_THAT(result, PairsFirstsEqual(reference));
    }
    SECTION("TBB Up-Down Sweep")
    {
        std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));
        std::copy(data.begin(), data.end(), result.begin());
        _tbb::updown::inclusive_segmented_scan(
            result.begin(), result.end(), result.begin());
        REQUIRE_THAT(result, PairsFirstsEqual(reference));
    }
    SECTION("TBB Tiled")
    {
        std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));
        std::copy(data.begin(), data.end(), result.begin());
        _tbb::tiled::inclusive_segmented_scan(
            result.begin(), result.end(), result.begin());
        REQUIRE_THAT(result, PairsFirstsEqual(reference));
    }
    // Does not work with provided scan functionality
    // SECTION("OpenMP provided")
    //{
    // std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));
    // openmp::provided::inclusive_segmented_scan(
    //  result.begin(), result.end(), result.begin());

    // std::vector<int> result(N);
    // for (size_t i = 0; i < N; i++)
    //{
    //  result[i] = result[i].first;
    //}
    //  REQUIRE_THAT(result, PairsFirstsEqual(reference));
    //}
    // SECTION("OpenMP Up-Down-Sweep")
    // {
    //     std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));
    //     std::copy(data.begin(), data.end(), result.begin());
    //     openmp::updown::inclusive_segmented_scan(
    //         result.begin(), result.end(), result.begin());
    //     REQUIRE_THAT(result, PairsFirstsEqual(reference));
    // }
    // SECTION("OpenMP Tiled")
    // {
    //     std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));
    //     std::copy(data.begin(), data.end(), result.begin());
    //     openmp::tiled::inclusive_segmented_scan(
    //         result.begin(), result.end(), result.begin());
    //     REQUIRE_THAT(result, PairsFirstsEqual(reference));
    // }
}

TEST_CASE("In-Place Exclusive Segmented Scan Sequential Test", "[in][exseg][seq]")
{
    // Test parameters
    const size_t N = 16;
    // Logging of parameters
    CAPTURE(N);

    std::vector<int> values({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16});
    std::vector<int> flags({0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0});
    std::vector<std::pair<int, int>> data(N);
    for (size_t i = 0; i < N; i++)
    {
        data[i] = std::make_pair(values[i], flags[i]);
    }

    std::vector<int> reference_val(
        {0, 1, 0, 3, 0, 5, 0, 7, 15, 24, 34, 0, 0, 0, 14, 29}); //, 0, 17, 35, 0});
    std::vector<std::pair<int, int>> reference(N);
    for (size_t i = 0; i < N; i++)
    {
        reference[i] = std::make_pair(reference_val[i], flags[i]);
    }

    //Tests
    SECTION("Sequential Naive")
    {
        std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));
        std::copy(data.begin(), data.end(), result.begin());
        sequential::naive::exclusive_segmented_scan(result.begin(), result.end(), 0);

        REQUIRE_THAT(result, PairsFirstsEqual(reference));
    }
}

TEST_CASE("In-Place Exclusive Segmented Scan Test", "[in][exseg]")
{
    // Test parameters
    const size_t N = GENERATE(logRange(1ull << 4, 1ull << 10, 2));

    // Logging of parameters
    CAPTURE(N);

    std::default_random_engine         generator;
    std::uniform_int_distribution<int> distribution(1, 10);
    auto                               randnum = std::bind(distribution, generator);

    std::default_random_engine         flag_generator;
    std::uniform_int_distribution<int> flag_distribution(0, 1);
    auto flag_rand = std::bind(flag_distribution, flag_generator);

    std::vector<std::pair<int, int>> data(N);
    std::generate(data.begin(),
                  data.end(),
                  [&randnum, &flag_rand]()
                  {
                      std::pair<int, int> A;
                      A.first  = randnum();
                      A.second = flag_rand();
                      return A;
                  });

    int init = 5;

    std::vector<std::pair<int, int>> reference(N);
    sequential::naive::exclusive_segmented_scan(
        data.begin(), data.end(), reference.begin(), init);

    SECTION("Sequential Up-Down-Sweep")
    {
        std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));
        std::copy(data.begin(), data.end(), result.begin());

        sequential::updown::exclusive_segmented_scan(
            result.begin(), result.end(), 0, init);

        REQUIRE_THAT(result, PairsFirstsEqual(reference));
    }
    SECTION("Sequential Tiled")
    {
        std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));
        std::copy(data.begin(), data.end(), result.begin());

        sequential::tiled::exclusive_segmented_scan(
            result.begin(), result.end(), 0, init);

        REQUIRE_THAT(result, PairsFirstsEqual(reference));
    }
    // Does not work with provided scan functionality
    // SECTION("OpenMP provided")
    //{
    //  std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));
    // openmp::provided::exclusive_segmented_scan(
    //  result.begin(), result.end(), result.begin());

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
        std::copy(data.begin(), data.end(), result.begin());

        openmp::updown::exclusive_segmented_scan(
            result.begin(), result.end(), result.begin(), 0, init);

        REQUIRE_THAT(result, PairsFirstsEqual(reference));
    }
    SECTION("OpenMP Tiled")
    {
        // std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));
        // std::copy(data.begin(), data.end(), result.begin());
        // openmp::tiled::exclusive_segmented_scan(
        //     result.begin(), result.end(), result.begin(), 0, init);
        // REQUIRE_THAT(result, PairsFirstsEqual(reference));
    }
    // SECTION("TBB provided")
    // {
    //     std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));
    //     std::copy(data.begin(), data.end(), result.begin());
    //     _tbb::provided::exclusive_segmented_scan(
    //         result.begin(), result.end(), result.begin(), init);
    //     REQUIRE_THAT(result, PairsFirstsEqual(reference));
    // }
    SECTION("TBB Up-Down Sweep")
    {
        // std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));
        // std::copy(data.begin(), data.end(), result.begin());
        // _tbb::updown::exclusive_segmented_scan(
        //     result.begin(), result.end(), result.begin(), 0, init);
        // REQUIRE_THAT(result, PairsFirstsEqual(reference));

        // std::cout << "data:" << std::endl;
        // std::for_each(data.begin(),
        //               data.end(),
        //               [](auto x)
        //               { std::cout << "(" << x.first << "|" << x.second << "), "; });
        // std::cout << std::endl;
        // std::cout << "result:" << std::endl;
        // std::for_each(result.begin(),
        //               result.end(),
        //               [](auto x)
        //               { std::cout << "(" << x.first << "|" << x.second << "), "; });
        // std::cout << std::endl;
    }
    SECTION("TBB Tiled")
    {
        // std::vector<std::pair<int, int>> result(N, std::make_pair(0, 0));
        // std::copy(data.begin(), data.end(), result.begin());
        // _tbb::tiled::exclusive_segmented_scan(
        //     result.begin(), result.end(), result.begin(), init);
        // REQUIRE_THAT(result, PairsFirstsEqual(reference));
    }
}
