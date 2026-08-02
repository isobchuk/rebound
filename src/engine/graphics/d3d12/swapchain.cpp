#include "engine\graphics\d3d12\swapchain.hpp"

using namespace isoeng::graphics::d3d12;

std::expected<SwapChain, SwapChain::Error> SwapChain::Create(const isoeng::pointer::win32::ComPtr<IDXGIFactory4> &f,
                                                             const isoeng::pointer::win32::ComPtr<ID3D12CommandQueue> &cq, HWND hwnd, const u32 width,
                                                             const u32 height) {
  using namespace isoeng::pointer::win32;
  using namespace isoeng::log;
  using enum Error;

  if (!f) [[unlikely]] {
    _log.error(string<"No valid factory provided!">);
    return std::unexpected(NO_VALID_FACTORY_PROVIDED);
  }

  if (!cq) [[unlikely]] {
    _log.error(string<"No valid command queue provided!">);
    return std::unexpected(NO_VALID_COMMAND_QUEUE_PROVIDED);
  }

  // Create descriptor
  const DXGI_SWAP_CHAIN_DESC1 descriptor{
      .Width = width,
      .Height = height,
      .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
      .Stereo = FALSE,
      .SampleDesc = {.Count = 1, .Quality = 0},
      .BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
      .BufferCount = Count(),
      .Scaling = DXGI_SCALING_STRETCH,
      .SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
      .AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED,
      .Flags = 0,
  };

  ComPtr<IDXGISwapChain1> swapChain1;

  auto hr = f.Get()->CreateSwapChainForHwnd(cq.Get(), hwnd, &descriptor, nullptr, nullptr, swapChain1.Out());
  if (FAILED(hr)) [[unlikely]] {
    _log.error(string<"Could not create swap chain with error [%X]!">, hr);
    return std::unexpected(CREATE_FAILED);
  }

  isoeng::pointer::win32::ComPtr<IDXGISwapChain4> swapChain4;
  if (FAILED(swapChain1.As(swapChain4))) [[unlikely]] {
    _log.error(string<"Could not convert swapChain1 to swapChain4!">);
    return std::unexpected(CONVERSION_FAILED);
  }

  if (f.Get()->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER)) [[unlikely]] {
    _log.error(string<"Could not disable Alt + Enter">);
    return std::unexpected(NO_ALT_ENTER_FAILED);
  }

  Buffers buffers;
  for (u32 i = 0; i < std::size(buffers); i++) {
    hr = swapChain4.Get()->GetBuffer(i, IID_PPV_ARGS(buffers[i].Out()));
    if (FAILED(hr)) [[unlikely]] {
      _log.error(string<"Could not get buffer [%u] with error [%X]!">, i, hr);
      return std::unexpected(Error::GET_BUFFER_FAILED);
    }
  }

  _log.info(string<"Swap chain was successfully created.">);
  return SwapChain(std::move(swapChain4), std::move(buffers));
}

SwapChain::SwapChain(isoeng::pointer::win32::ComPtr<IDXGISwapChain4> &&sc, Buffers &&bufs) : _chain(std::move(sc)), _buffers(std::move(bufs)) {}