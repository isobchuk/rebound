
#include "engine/graphics/d3d12/renderer.hpp"

using namespace isoeng::graphics::d3d12;

[[nodiscard]] std::expected<Renderer, Renderer::Error> Renderer::Create(HWND hwnd, const u32 width, const u32 height) {
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

  auto chain = SwapChain::Create(factory->Get(), queue->Get(), hwnd, width, height);
  if (!chain) {
    return std::unexpected(CREATION_ERROR_SWAP_CHAIN);
  }

  auto heap = RTVHeap::Create(device->Get());
  if (!heap) {
    return std::unexpected(CREATION_ERROR_DESCRIPTOR_HEAP);
  }

  auto allocator = Allocator::Create(device->Get());
  if (!allocator) {
    return std::unexpected(CREATION_ERROR_COMMAND_ALLOCATOR);
  }

  auto list = List::Create(device->Get(), allocator->Get());
  if (!list) {
    return std::unexpected(CREATION_ERROR_COMMAND_LIST);
  }

  return Renderer(std::move(*factory), std::move(*adapter), std::move(*device), std::move(*queue), std::move(*fence), std::move(*chain),
                  std::move(*heap), std::move(*allocator), std::move(*list));
}

Renderer::Renderer(Factory &&f, Adapter &&a, Device &&d, Queue &&q, Fence &&fe, SwapChain &&sc, RTVHeap &&h, Allocator &&al, List &&l)
    : _factory(std::move(f)), _adapter(std::move(a)), _device(std::move(d)), _queue(std::move(q)), _fence(std::move(fe)), _chain(std::move(sc)),
      _heap(std::move(h)), _allocator(std::move(al)), _list(std::move(l)) {

  // Create RTV for back buffers
  const auto &buffers = _chain.GetBuffers();
  for (u32 i = 0U; i < std::size(buffers); i++) {
    _device.Get().Get()->CreateRenderTargetView(buffers[i].Get(), nullptr, _heap.CpuHandle(i));
  }
}