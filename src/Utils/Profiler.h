#pragma once

#include <chrono>
#include <spdlog/spdlog.h>
#include <map>
#include <vector>
#include <numeric>

namespace utils {
class Profiler
{
public:
  explicit Profiler(std::string name)
    : _name(std::move(name)),
      _start_time(std::chrono::high_resolution_clock::now())
  {
  }

  ~Profiler()
  {
    const auto end_time     = std::chrono::high_resolution_clock::now();
    const auto time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - _start_time);
    //    spdlog::info("{} Done with {} ms.", _name, time_elapsed.count());
    _times[_name].push_back(time_elapsed.count());
  }

  static void dump(const std::string &name) noexcept
  {
    const auto &results = _times[name];

    long m = std::accumulate(results.begin(), results.end(), 0, std::plus<>());

    m /= results.size();

    spdlog::info("{} took in average [{} ms -- {} occurrences].", name, m, results.size());

    _times[name].clear();
  }


private:
  std::string                                                 _name;
  std::chrono::time_point<std::chrono::high_resolution_clock> _start_time;
  static std::map<std::string, std::vector<long>>             _times;
};

}// namespace utils
