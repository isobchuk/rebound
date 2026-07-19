#pragma once

#include <d3d12.h>

#include "engine\framework\log\log.hpp"

namespace isoeng::graphics::d3d12 {

class Device {
  static constexpr isoeng::log::Log _log{isoeng::log::log_lvl<isoeng::log::Trace::All>, isoeng::log::string<"engine.graphics.drd12.device">};
  ID3D12Device *_device;

public:
  Device();
};

} // namespace isoeng::graphics::d3d12