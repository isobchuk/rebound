#pragma once

#include "engine/graphics/d3d12/adapter.hpp"
#include "engine/graphics/d3d12/device.hpp"
#include "engine/graphics/d3d12/factory.hpp"

namespace isoeng::graphics::d3d12 {

class Renderer {
public:
  enum class Error { NO, CREATION_ERROR_FACTORY, CREATION_ERROR_ADAPTER };
  [[nodiscard]] static std::expected<Renderer, Error> Create();

private:
  const Factory _factory;
  const Adapter _adapter;
  isoeng::graphics::d3d12::Device _device;

  Renderer(const Factory &&f, const Adapter &&a);
};

} // namespace isoeng::graphics::d3d12