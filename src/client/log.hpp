#pragma once

#include <chrono>
#include <cstdio>

#include "engine/framework/log/log.hpp"

// TODO: Multiple threaded

namespace rebound::log {
class Puts final {
  const std::chrono::steady_clock::time_point start_;
  FILE *file_;

public:
  Puts();
  ~Puts();
  void puts(const char *buf) const;
  isoeng::usize tick() const;
};
} // namespace rebound::log