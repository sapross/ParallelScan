#include "csv_reporter.hpp"

namespace Catch
{
CsvReporter::CsvReporter(ReporterConfig const& _config): StreamingReporterBase(_config) {}

CsvReporter::~CsvReporter() = default;

std::string CsvReporter::getDescription() { return "Reports benchmark results as CSV"; }

std::string CsvReporter::getStylesheetRef() const { return std::string(); }

void CsvReporter::writeSourceInfo(SourceLineInfo const&) {}

#if defined(CATCH_CONFIG_ENABLE_BENCHMARKING)
void CsvReporter::benchmarkPreparing(std::string const& name)
{
    this->stream << name << '\t';
}

void CsvReporter::benchmarkStarting(BenchmarkInfo const&) {}

void CsvReporter::benchmarkEnded(BenchmarkStats<> const& benchmarkStats)
{
    this->stream << benchmarkStats.mean.point.count();
    for (auto&& info: lastInfo)
    {
        this->stream << "\t" << info.message;
    }
    this->stream << '\n';
    this->stream.flush();
}

void CsvReporter::benchmarkFailed(std::string const&) { this->stream << "failed\n"; }
#endif // CATCH_CONFIG_ENABLE_BENCHMARKING

CATCH_REGISTER_REPORTER("csv", CsvReporter)
} // end namespace Catch
