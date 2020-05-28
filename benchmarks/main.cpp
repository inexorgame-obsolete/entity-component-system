#include <benchmark/benchmark.h>
#include <spdlog/spdlog.h>

int main(int argc, char **argv) {
    spdlog::set_level(spdlog::level::off);

    benchmark::Initialize(&argc, argv);
    if (benchmark::ReportUnrecognizedArguments(argc, argv)) {
        return 1;
    }

    benchmark::RunSpecifiedBenchmarks();
}
