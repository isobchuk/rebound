
#include "engine/graphics/d3d12/renderer.hpp"

using namespace isoeng::graphics::d3d12;

[[nodiscard]] std::expected<Renderer, Renderer::Error> Renderer::Create() {
  using enum Error;
  using namespace isoeng::pointer::win32;

  ComPtr<ID3D12Debug> debug;

  if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(debug.Out())))) {
    debug.Get()->EnableDebugLayer();
  }

  auto factory = Factory::Create();
  if (!factory) {
    return std::unexpected(CREATION_ERROR_FACTORY);
  }

  auto adapter = Adapter::Create(factory->Get());
  if (!adapter) {
    return std::unexpected(CREATION_ERROR_ADAPTER);
  }

  auto device = Device::Create(adapter->Get(), adapter->Info().level);
  if (!device) {
    return std::unexpected(CREATION_ERROR_DEVICE);
  }

  auto queue = Queue::Create(device->Get());
  if (!queue) {
    return std::unexpected(CREATION_ERROR_COMMAND_QUEUE);
  }

  auto fence = Fence::Create(device->Get());
  if (!fence) {
    return std::unexpected(CREATION_ERROR_FENCE);
  }

  return Renderer(std::move(*factory), std::move(*adapter), std::move(*device), std::move(*queue), std::move(*fence));
}

Renderer::Renderer(Factory &&f, Adapter &&a, Device &&d, Queue &&q, Fence &&fe)
    : _factory(std::move(f)), _adapter(std::move(a)), _device(std::move(d)), _queue(std::move(q)), _fence(std::move(fe)) {}