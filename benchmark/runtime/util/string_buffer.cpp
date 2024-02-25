#include <string>
#include <sstream>

#include <benchmark/benchmark.h>

#include <librepr/util/string/buffer.h>

namespace librepr::util::benchmark {
void string_append(::benchmark::State& state) {
  for (auto _ : state) {
    std::string buffer{};
    for (unsigned i = 0; i < 100; ++i){
      buffer.append("fooabc1234567890 asdf <>()");
    }

    std::string result = buffer;
    ::benchmark::DoNotOptimize(result);
    ::benchmark::ClobberMemory();
  }
}
BENCHMARK(string_append);

void stringstream_append(::benchmark::State& state) {
  for (auto _ : state) {
    std::ostringstream buffer{};
    for (unsigned i = 0; i < 100; ++i){
      buffer << "fooabc1234567890 asdf <>()";
    }

    std::string result = buffer.str();
    ::benchmark::DoNotOptimize(result);
    ::benchmark::ClobberMemory();
  }
}
BENCHMARK(stringstream_append);

void stringbuffer_append(::benchmark::State& state) {
  for (auto _ : state) {
    librepr::util::StringBuffer buffer{};
    for (unsigned i = 0; i < 100; ++i){
      buffer << "fooabc1234567890 asdf <>()";
    }

    std::string result = buffer.extract();
    ::benchmark::DoNotOptimize(result);
    ::benchmark::ClobberMemory();
  }
}
BENCHMARK(stringbuffer_append);

}