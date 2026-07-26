#pragma once

#include "engine\graphics\d3d12\adapter.hpp"
#include "engine\graphics\d3d12\device.hpp"
#include "engine\graphics\d3d12\factory.hpp"
#include "engine\graphics\d3d12\fence.hpp"
#include "engine\graphics\d3d12\queue.hpp"

namespace isoeng::graphics::d3d12 {

class Renderer {
public:
  enum class Error { NO, CREATION_ERROR_FACTORY, CREATION_ERROR_ADAPTER, CREATION_ERROR_DEVICE, CREATION_ERROR_COMMAND_QUEUE, CREATION_ERROR_FENCE };
  [[nodiscard]] static std::expected<Renderer, Error> Create();

private:
  const Factory _factory;
  const Adapter _adapter;
  const Device _device;
  const Queue _queue;
  const Fence _fence;

  Renderer(Factory &&f, Adapter &&a, Device &&d, Queue &&q, Fence &&fe);
};

} // namespace isoeng::graphics::d3d12