#pragma once

#include <expected>

#include <d3d12.h>
#include <dxgi1_6.h>

#include "engine\framework\log\log.hpp"
#include "engine\platform\pointer\win32\com.hpp"

namespace isoeng::graphics::d3d12 {

class Device {
public:
  enum class Error { NO, CREATE_FAILED, NO_VALID_ADAPTER_PROVIDED };

  [[nodiscard]] static std::expected<Device, Error> Create(const isoeng::pointer::win32::ComPtr<IDXGIAdapter4> &a,
                                                           const D3D_FEATURE_LEVEL lvl = D3D_FEATURE_LEVEL_11_0);

  [[nodiscard]] inline const isoeng::pointer::win32::ComPtr<ID3D12Device> &Get() const noexcept { return _device; }

private:
  static constexpr isoeng::log::Log _log{isoeng::log::log_lvl<isoeng::log::Trace::All>, isoeng::log::string<"engine.graphics.d3d12.device">};

  const isoeng::pointer::win32::ComPtr<ID3D12Device> _device;

  Device(isoeng::pointer::win32::ComPtr<ID3D12Device> &&d);
};

} // namespace isoeng::graphics::d3d12