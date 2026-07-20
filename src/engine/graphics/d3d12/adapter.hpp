#pragma once

#include <dxgi1_6.h>

#include "engine/graphics/d3d12/factory.hpp"
#include "engine\framework\log\log.hpp"
#include "engine\platform\pointer\win32\com.hpp"

namespace isoeng::graphics::d3d12 {

class Adapter {

public:
  enum class Error { NO };

  [[nodiscard]] static std::expected<Adapter, Error> Create(const isoeng::pointer::win32::ComPtr<IDXGIFactory4> &f);

private:
  static constexpr isoeng::log::Log _log{isoeng::log::log_lvl<isoeng::log::Trace::All>, isoeng::log::string<"engine.graphics.drd12.adapter">};

  isoeng::pointer::win32::ComPtr<IDXGIAdapter4> _adapter;

  /*std::string _name;
  usize _memory;
  u32 _number;*/

  Adapter();
};
} // namespace isoeng::graphics::d3d12