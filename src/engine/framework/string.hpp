#pragma once

#include "engine/platform/arch/types.hpp"

namespace isoeng {
template <const isoeng::usize N> struct ASCIIString final {
public:
  ASCIIString() = default;
  ASCIIString(const wchar_t (&src)[N]) {
    using namespace isoeng;

    for (usize i = 0UL; i < N; i++) {
      string[i] = src[i] <= 0x7FU ? static_cast<const char>(src[i]) : '?';
    }
  }

  ASCIIString(const ASCIIString &) noexcept = default;
  ASCIIString(ASCIIString &&) noexcept = default;

  ASCIIString &operator=(const ASCIIString &) noexcept = default;
  ASCIIString &operator=(ASCIIString &&) noexcept = default;

  inline auto c_str() const -> const char (&)[N] { return string; }
  static consteval isoeng::usize size() noexcept { return N; }

private:
  char string[N];
};
} // namespace isoeng