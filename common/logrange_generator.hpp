#include <catch2/catch.hpp>

class LogRange: public Catch::Generators::IGenerator<std::uint64_t>
{
  std::uint64_t curr_;
  std::uint64_t end_;
  std::uint64_t log_;

  public:
    LogRange(std::uint64_t start, std::uint64_t end, std::uint64_t log): curr_(start), end_(end), log_(log) {}

    // via GeneratorUntypedBase:
    // Attempts to move the generator to the next element.
    // Returns true if successful (and thus has another element that can be read)
    bool next() override
    {
        curr_ = curr_ * log_;
        return curr_ < end_;
    };

    // Precondition:
    // The generator is either freshly constructed or the last call to next() returned true
    std::uint64_t const& get() const override { return curr_; };
};
Catch::Generators::GeneratorWrapper<std::uint64_t> logRange(std::uint64_t start, std::uint64_t end, std::uint64_t log)
{
    return Catch::Generators::GeneratorWrapper<std::uint64_t>(
        std::unique_ptr<Catch::Generators::IGenerator<std::uint64_t>>(new LogRange(start, end, log)));
}