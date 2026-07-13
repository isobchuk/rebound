#pragma once

#include "engine/graphics/d3d12/device.hpp"

namespace isoeng::graphics::d3d12 {

class Renderer {
  isoeng::graphics::d3d12::Device _device;

public:
  Renderer();
};

} // namespace isoeng::graphics::d3d12