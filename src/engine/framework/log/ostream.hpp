#pragma once

#include <ostream>

// Basic namespace to work with log component
namespace isoeng::log {

class Ostream final {
public:
  [[nodiscard]] inline static const Ostream &Init(std::ostream *ostr) noexcept {
    static Ostream ostream(ostr);
    return ostream;
  }

  [[nodiscard]] inline static std::ostream *Get() noexcept { return _stream; }

private:
  inline Ostream(std::ostream *ostr) noexcept { _stream = ostr; }
  static std::ostream *_stream;
};

} // namespace isoeng::log