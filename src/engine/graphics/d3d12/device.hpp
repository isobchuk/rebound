#pragma once

#include <d3d12.h>

namespace isoeng::graphics::d3d12 {

class Device {
  ID3D12Device *_device;

public:
  Device();
};

} // namespace isoeng::graphics::d3d12