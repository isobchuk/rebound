#include "engine/graphics/d3d12/renderer.hpp"

using namespace isoeng::graphics::d3d12;

[[nodiscard]] std::expected<Renderer, Renderer::Error> Renderer::Create() {
  using enum Error;

  const auto factory = Factory::Create();
  if (!factory) {
    return std::unexpected(NO); // TODO
  }

  const auto adapter = Adapter::Create((*factory).Get());
  if (!adapter) {
    return std::unexpected(NO); // TODO
  }

  return Renderer(std::move(*factory), std::move(*adapter));
}

Renderer::Renderer(const Factory &&f, const Adapter &&a) : _factory(std::move(f)), _adapter(std::move(a)), _device() {}
