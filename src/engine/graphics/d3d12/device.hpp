#pragma once

#include <d3d12.h>

#include "engine\framework\log\log.hpp"
#include "engine\platform\pointer\win32\com.hpp"

namespace isoeng::graphics::d3d12 {

class Device {
  static constexpr isoeng::log::Log _log{isoeng::log::log_lvl<isoeng::log::Trace::All>, isoeng::log::string<"engine.graphics.drd12.device">};

  isoeng::pointer::win32::ComPtr<ID3D12Device> _device;

public:
  Device();
};

} // namespace isoeng::graphics::d3d12