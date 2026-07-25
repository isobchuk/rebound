#include "engine/graphics/d3d12/device.hpp"

using namespace isoeng::graphics::d3d12;

Device::Device() : _device() {
  using namespace isoeng::log;

  const auto hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(_device.Out()));

  if (FAILED(hr)) [[unlikely]] {
    _log.error(string<"Could not create d3d12 device with error %d">, hr);
  }
}