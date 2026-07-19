#pragma once

#include "engine/graphics/d3d12/adapter.hpp"
#include "engine/graphics/d3d12/device.hpp"
#include "engine/graphics/d3d12/factory.hpp"

namespace isoeng::graphics::d3d12 {

class Renderer {
  isoeng::graphics::d3d12::Factory _factory;
  isoeng::graphics::d3d12::Adapter _adapter;
  isoeng::graphics::d3d12::Device _device;

public:
  Renderer();
};

} // namespace isoeng::graphics::d3d12