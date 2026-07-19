#pragma once

#include <dxgi1_6.h>

#include "engine\framework\log\log.hpp"
#include "engine\platform\pointer\win32\com.hpp"

namespace isoeng::graphics::d3d12 {

class Factory {

public:
  Factory();

  [[nodiscard]] inline isoeng::pointer::win32::ComPtr<IDXGIFactory4> &Get() noexcept { return _factory; }

private:
  static constexpr isoeng::log::Log _log{isoeng::log::log_lvl<isoeng::log::Trace::All>, isoeng::log::string<"engine.graphics.drd12.factory">};

  isoeng::pointer::win32::ComPtr<IDXGIFactory4> _factory;
};
} // namespace isoeng::graphics::d3d12