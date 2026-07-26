#pragma once

#pragma once

#pragma once

#include <expected>

#include <d3d12.h>

#include "engine\framework\log\log.hpp"
#include "engine\platform\pointer\win32\com.hpp"

namespace isoeng::graphics::d3d12 {

class Fence {
public:
  enum class Error { NO, CREATE_FAILED, NO_VALID_DEVICE_PROVIDED };

  [[nodiscard]] static std::expected<Fence, Error> Create(const isoeng::pointer::win32::ComPtr<ID3D12Device> &d);

  [[nodiscard]] inline const isoeng::pointer::win32::ComPtr<ID3D12Fence> &Get() const noexcept { return _fence; }

private:
  static constexpr isoeng::log::Log _log{isoeng::log::log_lvl<isoeng::log::Trace::All>, isoeng::log::string<"engine.graphics.d3d12.fence">};

  const isoeng::pointer::win32::ComPtr<ID3D12Fence> _fence;

  Fence(isoeng::pointer::win32::ComPtr<ID3D12Fence> &&f);
};

} // namespace isoeng::graphics::d3d12