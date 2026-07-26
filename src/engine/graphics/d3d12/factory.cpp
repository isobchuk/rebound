#include "engine/graphics/d3d12/factory.hpp"

using namespace isoeng::graphics::d3d12;

[[nodiscard]] std::expected<Factory, Factory::Error> Factory::Create() {
  using namespace isoeng::pointer::win32;
  using namespace isoeng::log;
  using enum Error;

  constexpr auto FLAGS = 0U;

  ComPtr<IDXGIFactory4> factory;
  const auto hr = CreateDXGIFactory2(FLAGS, IID_PPV_ARGS(factory.Out()));

  if (FAILED(hr)) {
    _log.error(string<"Could not create DXDI factory with hr [%X]!">, hr);
    return std::unexpected(CREATE_FAILED);
  }

  return Factory(std::move(factory));
}

Factory::Factory(isoeng::pointer::win32::ComPtr<IDXGIFactory4> &&f) : _factory(std::move(f)) {}