
#include "engine/graphics/d3d12/renderer.hpp"

using namespace isoeng::graphics::d3d12;

[[nodiscard]] std::expected<Renderer, Renderer::Error> Renderer::Create() {
  using enum Error;
  using namespace isoeng::pointer::win32;

  ComPtr<ID3D12Debug> debug;

  if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(debug.Out())))) {
    debug.Get()->EnableDebugLayer();
  }

  const auto factory = Factory::Create();
  if (!factory) {
    return std::unexpected(CREATION_ERROR_FACTORY);
  }

  const auto adapter = Adapter::Create((*factory).Get());
  if (!adapter) {
    return std::unexpected(CREATION_ERROR_ADAPTER); // TODO
  }

  return Renderer(std::move(*factory), std::move(*adapter));
}

Renderer::Renderer(const Factory &&f, const Adapter &&a) : _factory(std::move(f)), _adapter(std::move(a)), _device() {}