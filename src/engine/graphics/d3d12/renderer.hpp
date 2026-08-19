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
#include "engine\graphics\shader.hpp"

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

  template <std::unsigned_integral U = u32> struct WindowSize {
    const U _width;
    const U _height;

    constexpr WindowSize(const U w, const U h) : _width(w), _height(h) {}
  };
  using RenderWindowSize = WindowSize<>;

  [[nodiscard]] static std::expected<Renderer, Error> Create(HWND hwnd, const u32 width, const u32 height);

private:
  static constexpr isoeng::log::Log _log{isoeng::log::log_lvl<isoeng::log::Trace::All>, isoeng::log::string<"engine.graphics.d3d12.renderer">};

  static constexpr List::Color _BLACK{0.0f, 0.0f, 0.0f, 1.0f};
  static constexpr List::Color _WHITE{1.0f, 1.0f, 1.0f, 1.0f};

  static constexpr List::Color _RED{1.0f, 0.0f, 0.0f, 1.0f};
  static constexpr List::Color _GREEN{0.0f, 1.0f, 0.0f, 1.0f};
  static constexpr List::Color _BLUE{0.0f, 0.0f, 1.0f, 1.0f};

  static constexpr List::Color _YELLOW{1.0f, 1.0f, 0.0f, 1.0f};
  static constexpr List::Color _CYAN{0.0f, 1.0f, 1.0f, 1.0f};
  static constexpr List::Color _MAGENTA{1.0f, 0.0f, 1.0f, 1.0f};

  static constexpr List::Color _ORANGE{1.0f, 0.5f, 0.0f, 1.0f};
  static constexpr List::Color _PURPLE{0.5f, 0.0f, 1.0f, 1.0f};
  static constexpr List::Color _PINK{1.0f, 0.4f, 0.7f, 1.0f};

  static constexpr List::Color _GRAY{0.5f, 0.5f, 0.5f, 1.0f};
  static constexpr List::Color _DARK_GRAY{0.2f, 0.2f, 0.2f, 1.0f};
  static constexpr List::Color _LIGHT_GRAY{0.8f, 0.8f, 0.8f, 1.0f};

  using RTVHeap = RTVHeapDescriptor<SwapChain::Count()>;

  const Factory _factory;
  const Adapter _adapter;
  const Device _device;
  const Queue _queue;
  Fence _fence;
  const SwapChain _chain;
  const RTVHeap _heap;
  const Allocator _allocator;
  const List _list;

  const RenderWindowSize _size;

  template <Shader::Type type> using ShaderFile = Shader::File<isoeng::log::string<"shaders">, type>;

  Renderer(RenderWindowSize size, Factory &&f, Adapter &&a, Device &&d, Queue &&q, Fence &&fe, SwapChain &&sc, RTVHeap &&h, Allocator &&al, List &&l);
};

} // namespace isoeng::graphics::d3d12