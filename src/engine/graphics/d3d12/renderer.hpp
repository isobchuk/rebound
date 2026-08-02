#pragma once

#include "engine\graphics\d3d12\adapter.hpp"
#include "engine\graphics\d3d12\allocator.hpp"
#include "engine\graphics\d3d12\descheap.hpp"
#include "engine\graphics\d3d12\device.hpp"
#include "engine\graphics\d3d12\factory.hpp"
#include "engine\graphics\d3d12\fence.hpp"
#include "engine\graphics\d3d12\list.hpp"
#include "engine\graphics\d3d12\queue.hpp"
#include "engine\graphics\d3d12\swapchain.hpp"

namespace isoeng::graphics::d3d12 {

class Renderer {
public:
  enum class Error {
    NO,
    CREATION_ERROR_FACTORY,
    CREATION_ERROR_ADAPTER,
    CREATION_ERROR_DEVICE,
    CREATION_ERROR_COMMAND_QUEUE,
    CREATION_ERROR_FENCE,
    CREATION_ERROR_SWAP_CHAIN,
    CREATION_ERROR_DESCRIPTOR_HEAP,
    CREATION_ERROR_COMMAND_ALLOCATOR,
    CREATION_ERROR_COMMAND_LIST
  };

  [[nodiscard]] static std::expected<Renderer, Error> Create(HWND hwnd, const u32 width, const u32 height);

private:
  using RTVHeap = RTVHeapDescriptor<SwapChain::Count()>;

  const Factory _factory;
  const Adapter _adapter;
  const Device _device;
  const Queue _queue;
  const Fence _fence;
  const SwapChain _chain;
  const RTVHeap _heap;
  const Allocator _allocator;
  const List _list;

  Renderer(Factory &&f, Adapter &&a, Device &&d, Queue &&q, Fence &&fe, SwapChain &&sc, RTVHeap &&h, Allocator &&al, List &&l);
};

} // namespace isoeng::graphics::d3d12