#pragma once

#include <array>
#include <expected>

#include <d3d12.h>
#include <dxgi1_5.h>

#include "engine\framework\log\log.hpp"
#include "engine\platform\pointer\win32\com.hpp"

namespace isoeng::graphics::d3d12 {

class SwapChain {
public:
  enum class Error {
    NO,
    CREATE_FAILED,
    NO_VALID_FACTORY_PROVIDED,
    NO_VALID_COMMAND_QUEUE_PROVIDED,
    CONVERSION_FAILED,
    NO_ALT_ENTER_FAILED,
    GET_BUFFER_FAILED,
    PRESENT_FAILED
  };

  [[nodiscard]] static std::expected<SwapChain, Error> Create(const isoeng::pointer::win32::ComPtr<IDXGIFactory4> &f,
                                                              const isoeng::pointer::win32::ComPtr<ID3D12CommandQueue> &cq, HWND hwnd,
                                                              const u32 width, const u32 height);

  [[nodiscard]] inline const isoeng::pointer::win32::ComPtr<IDXGISwapChain4> &Get() const noexcept { return _chain; }

  using Buffers = std::array<isoeng::pointer::win32::ComPtr<ID3D12Resource>, 2U>; // double buffering
  [[nodiscard]] inline static consteval auto Count() noexcept { return std::tuple_size_v<Buffers>; }
  [[nodiscard]] inline const Buffers &GetBuffers() const noexcept { return _buffers; }

  [[nodiscard]] Error Present() const noexcept;

private:
  static constexpr isoeng::log::Log _log{isoeng::log::log_lvl<isoeng::log::Trace::All>, isoeng::log::string<"engine.graphics.d3d12.chain">};

  const isoeng::pointer::win32::ComPtr<IDXGISwapChain4> _chain;
  const Buffers _buffers;

  SwapChain(isoeng::pointer::win32::ComPtr<IDXGISwapChain4> &&sc, Buffers &&bufs);
};

} // namespace isoeng::graphics::d3d12