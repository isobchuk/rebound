#pragma once

#include <concepts>
#include <type_traits>

#include "engine/platform/arch/types.hpp"

namespace isoeng::log {

/**
 * @brief Concept to check that type is String
 *
 * @tparam S The type should be checked
 */
template <typename S>
concept const_string = requires(S) {
  typename S::StringT;
  S::string;
};

/**
 * @brief Trait to check that type is String
 *
 * @tparam T
 */
template <typename T, typename U = void> struct is_string {
  static constexpr auto value = false;
};
template <typename T> struct is_string<T, typename T::StringT::type> {
  static constexpr auto value = true;
};
template <typename T> inline constexpr auto is_string_v = is_string<T>::value;

// Contains some wrappers that is suggested as additional for the external users
namespace wrappers {
/**
 * @brief Struct that saves a string in array inside constexpr object
 */
template <const usize N> struct Wrap {
  static constexpr auto size = N; // Size of the array
  char elems[N];                  // Array that contains string

  /**
   * @brief   Compile-time constructor that saves string in the array
   *
   * @param s Reference to an array (usually just const char[])
   */
  consteval Wrap(const char (&s)[N]) {
    usize i = 0;
    while (i < N) {
      elems[i] = s[i];
      i++;
    }
  }

  /**
   * @brief     Concatenate the source string with the passed
   *
   * @tparam S  struct Wrap
   * @param s   object
   * @return    New consteval Wrap object
   */
  template <typename S> consteval auto operator+(const S s) const {
    char str[size - 1 + S::size];
    usize i = 0;
    while (i < (size - 1)) {
      str[i] = elems[i];
      i++;
    }
    i = 0;
    while (i < S::size) {
      str[i + (size - 1)] = s.elems[i];
      i++;
    }
    return Wrap<size - 1 + S::size>(str);
  }
};
// Deduction guide for the Wrap
template <const usize N> Wrap(char const (&)[N]) -> Wrap<N>;

/**
 * @brief Type that transform array from Wrap to the type property
 *
 */
template <const auto str> struct String {
  static constexpr auto &instance = str;     // Instance of Wrap
  static constexpr auto &string = str.elems; // Reference to an array
  struct StringT {
    using type = void;
  };

  /**
   * @brief   Concatenate the source string with the passed
   *
   * @tparam  S String type
   * @return  New consteval String object
   */
  template <const_string S> consteval auto operator+(const S) const { return String<instance + S::instance>{}; }

  /**
   * @brief   Returns raw pointer to an array
   *
   * @return  Pointer to char
   */
  consteval auto c_str() const { return string; }
};
} // namespace wrappers

/**
 * @brief   Inline variable that supposed to be used to create strings
 *
 * @example string<"This is string!">, constexpr auto my_str = string<"My string!!!">
 */
template <const wrappers::Wrap str> inline constexpr auto string = wrappers::String<str>{};

/**
 * @brief   Type for string
 */
template <const wrappers::Wrap str> using String = wrappers::String<str>;

template <const isoeng::usize N> struct ASCIIString final {
public:
  ASCIIString() = default;
  ASCIIString(const wchar_t (&src)[N]) {
    using namespace isoeng;

    for (usize i = 0UL; i < N; i++) {
      string[i] = src[i] <= 0x7FU ? static_cast<const char>(src[i]) : '?';
    }
  }

  inline auto c_str() const -> const char (&)[N] { return string; }
  static consteval isoeng::usize size() noexcept { return N; }

private:
  char string[N];
};

} // namespace isoeng::log