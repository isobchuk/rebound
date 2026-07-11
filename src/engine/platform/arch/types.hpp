#pragma once

#include <cstddef>
#include <cstdint>
#include <uchar.h>

namespace isoeng {

// Byte
using byte = std::byte;

// Integer types unsigned
using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

// Integer types signed
using i8 = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

// Pointer-sized integers
using uptr = std::uintptr_t;
using iptr = std::intptr_t;

// Size types
using usize = std::size_t;
using isize = std::ptrdiff_t;

// Floating point
using f32 = float;
using f64 = double;

// Character types
using c8 = char8_t;   // UTF-8 code unit
using c16 = char16_t; // UTF-16 code unit
using c32 = char32_t; // UTF-32 code unit

} // namespace isoeng