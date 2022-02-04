#pragma once

#include <catch2/catch.hpp>

namespace Catch
{
class CsvReporter: public StreamingReporterBase<CsvReporter>
{
  public:
    CsvReporter(ReporterConfig const& _config);

    ~CsvReporter() override;

    static std::string getDescription();

    virtual std::string getStylesheetRef() const;

    void writeSourceInfo(SourceLineInfo const& sourceInfo);

  public: // StreamingReporterBase
    void assertionStarting(AssertionInfo const&) override {}

    bool assertionEnded(AssertionStats const& assertionStats) override
    {
        lastInfo = assertionStats.infoMessages;
        return true;
    }

#if defined(CATCH_CONFIG_ENABLE_BENCHMARKING)
    void benchmarkPreparing(std::string const& name) override;
    void benchmarkStarting(BenchmarkInfo const&) override;
    void benchmarkEnded(BenchmarkStats<> const&) override;
    void benchmarkFailed(std::string const&) override;
#endif // CATCH_CONFIG_ENABLE_BENCHMARKING

  private:
    std::vector<MessageInfo> lastInfo;
};
} // namespace Catch
