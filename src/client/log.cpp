

#include "client/log.hpp"

using namespace rebound::log;

Puts::Puts() : start_(std::chrono::steady_clock::now()), file_(nullptr) { fopen_s(&file_, "rebound.log", "w"); }

Puts::~Puts() {
  if (file_) [[likely]] {
    fclose(file_);
  }
}

void Puts::puts(const char *buf) const {
  if (file_) [[likely]] {
    fputs(buf, file_);
  }
}

isoeng::u64 Puts::tick() const {
  using namespace isoeng;

  const auto now = std::chrono::steady_clock::now();
  return static_cast<u64>(std::chrono::duration_cast<std::chrono::milliseconds>(now - start_).count());
}