#include "engine/graphics/d3d12/device.hpp"

using namespace isoeng::graphics::d3d12;

Device::Device() : _device() {
  const auto hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&_device));
  if (FAILED(hr)) {
  }
}