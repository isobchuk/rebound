
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
  using namespace isoeng::log;

  // Create RTV for back buffers
  const auto &buffers = _chain.GetBuffers();
  for (u32 i = 0U; i < std::size(buffers); i++) {
    _device.Get().Get()->CreateRenderTargetView(buffers[i].Get(), nullptr, _heap.CpuHandle(i));
  }

  // Start pipeline
  _log.info(string<"Start Render.">);

  // Get index of the current back buffer
  const auto index = _chain.Get().Get()->GetCurrentBackBufferIndex();

  // Reset command list and command allocator
  auto res = _list.Begin(_allocator.Get());
  if (List::Error::NO != res) [[unlikely]] {
    _log.error(string<"Reset command list failed with error [%d]!">, static_cast<std::underlying_type_t<decltype(res)>>(res));
    return;
  }

  // Change state from Present to Render
  res = _list.Transition(buffers[index], {D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET});
  if (List::Error::NO != res) [[unlikely]] {
    _log.error(string<"Transition from PRESENT to RENDER failed [%d]!">, static_cast<std::underlying_type_t<decltype(res)>>(res));
    return;
  }

  // Set target buffer
  const auto handle = _heap.CpuHandle(index);
  _list.SetRenderTarget(handle);

  // Fill Window with color
  _list.Clear(handle, _ORANGE);

  // Change state from Render to Present
  res = _list.Transition(buffers[index], {D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT});
  if (List::Error::NO != res) [[unlikely]] {
    _log.error(string<"End command list failed [%d]!">, static_cast<std::underlying_type_t<decltype(res)>>(res));
    return;
  }

  // End write the command list
  res = _list.End();
  if (List::Error::NO != res) [[unlikely]] {
    _log.error(string<"Transition from PRESENT to RENDER failed [%d]!">, static_cast<std::underlying_type_t<decltype(res)>>(res));
    return;
  }

  // Execute command list
  if (Queue::Error::NO != _queue.Execute(_list.Get())) {
    _log.error(string<"Execute command list failed!">);
    return;
  }

  // Swap buffers
  if (SwapChain::Error::NO != _chain.Present()) {
    _log.error(string<"Present buffer failed!">);
    return;
  }

  // Signal
  if (Fence::Error::NO != _fence.Signal(_queue.Get())) {
    _log.error(string<"Fence Signal failed!">);
    return;
  }

  // Wait
  if (Fence::Error::NO != _fence.Wait()) {
    _log.error(string<"Fence Wait failed!">);
    return;
  }

  // Stop pipeline
  _log.info(string<"Stop Render.">);
}