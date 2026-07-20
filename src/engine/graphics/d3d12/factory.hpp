#pragma once

#include <expected>

#include <dxgi1_6.h>

#include "engine\framework\log\log.hpp"
#include "engine\platform\pointer\win32\com.hpp"

namespace isoeng::graphics::d3d12 {

class Factory {
public:
  enum class Error { NO, ERROR_CREATION };

  [[nodiscard]] static std::expected<Factory, Error> Create();

  [[nodiscard]] inline const isoeng::pointer::win32::ComPtr<IDXGIFactory4> &Get() const noexcept { return _factory; }

private:
  static constexpr isoeng::log::Log _log{isoeng::log::log_lvl<isoeng::log::Trace::All>, isoeng::log::string<"engine.graphics.drd12.factory">};

  const isoeng::pointer::win32::ComPtr<IDXGIFactory4> _factory;

  Factory(const isoeng::pointer::win32::ComPtr<IDXGIFactory4> f);
};
} // namespace isoeng::graphics::d3d12