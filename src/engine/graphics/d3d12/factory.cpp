#include "engine/graphics/d3d12/factory.hpp"

using namespace isoeng::graphics::d3d12;

[[nodiscard]] std::expected<Factory, Factory::Error> Factory::Create() {
  using namespace isoeng::pointer::win32;
  using enum Error;

  constexpr auto FLAGS = 0U;

  ComPtr<IDXGIFactory4> factory;
  if (FAILED(CreateDXGIFactory2(FLAGS, IID_PPV_ARGS(&factory)))) {
    return std::unexpected(ERROR_CREATION);
  }

  return Factory(std::move(factory));
}

Factory::Factory(const isoeng::pointer::win32::ComPtr<IDXGIFactory4> f) : _factory(std::move(f)) {}