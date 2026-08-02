#pragma once

#include <expected>

#include <d3d12.h>

#include "engine\framework\log\log.hpp"
#include "engine\platform\pointer\win32\com.hpp"

namespace isoeng::graphics::d3d12 {

class Allocator {
public:
  enum class Error { NO, CREATE_FAILED, NO_VALID_DEVICE_PROVIDED };

  [[nodiscard]] static std::expected<Allocator, Error> Create(const isoeng::pointer::win32::ComPtr<ID3D12Device> &d);

  [[nodiscard]] inline const isoeng::pointer::win32::ComPtr<ID3D12CommandAllocator> &Get() const noexcept { return _allocator; }

private:
  static constexpr isoeng::log::Log _log{isoeng::log::log_lvl<isoeng::log::Trace::All>, isoeng::log::string<"engine.graphics.d3d12.allocator">};

  const isoeng::pointer::win32::ComPtr<ID3D12CommandAllocator> _allocator;

  Allocator(isoeng::pointer::win32::ComPtr<ID3D12CommandAllocator> &&a);
};

} // namespace isoeng::graphics::d3d12