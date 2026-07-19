#pragma once

#include <chrono>

#include "engine/platform/arch/types.hpp"

// Basic namespace to work with log component
namespace isoeng::log {

class Time final {
  using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;

public:
  [[nodiscard]] inline static isoeng::u64 Get() noexcept {
    using namespace isoeng;
    return static_cast<u64>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - _start).count());
  }

private:
  Time() = delete;

  static const TimePoint _start;
};

} // namespace isoeng::log