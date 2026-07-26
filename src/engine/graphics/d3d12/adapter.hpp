#pragma once

#include <expected>

#include <d3dcommon.h>
#include <dxgi1_6.h>


#include "engine\framework\log\log.hpp"
#include "engine\platform\pointer\win32\com.hpp"

namespace isoeng::graphics::d3d12 {

class Adapter {

public:
  enum class Error { NO, ADAPTER_NOT_FOUND, NO_VALID_FACTORY_PROVIDED };

  struct AdapterInfo {
    using Name = ASCIIString<128U>;
    Name name;
    usize memory;
    D3D_FEATURE_LEVEL level;
    isoeng::pointer::win32::ComPtr<IDXGIAdapter1> adapter1;

    inline AdapterInfo(const Name &n, usize m, isoeng::pointer::win32::ComPtr<IDXGIAdapter1> &&a)
        : name(n), memory(m), level(D3D_FEATURE_LEVEL_11_0), adapter1(std::move(a)) {}
  };

  [[nodiscard]] static std::expected<Adapter, Error> Create(const isoeng::pointer::win32::ComPtr<IDXGIFactory4> &f);

  [[nodiscard]] inline const isoeng::pointer::win32::ComPtr<IDXGIAdapter4> &Get() const noexcept { return _adapter; }
  [[nodiscard]] inline const AdapterInfo &Info() const noexcept { return _info; }

private:
  static constexpr isoeng::log::Log _log{isoeng::log::log_lvl<isoeng::log::Trace::All>, isoeng::log::string<"engine.graphics.d3d12.adapter">};

  const isoeng::pointer::win32::ComPtr<IDXGIAdapter4> _adapter;
  const AdapterInfo _info;

  Adapter(isoeng::pointer::win32::ComPtr<IDXGIAdapter4> &&a, AdapterInfo &&info);
};
} // namespace isoeng::graphics::d3d12