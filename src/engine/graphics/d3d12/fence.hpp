#pragma once

#include <expected>

#include <d3d12.h>

#include "engine\framework\log\log.hpp"
#include "engine\platform\pointer\win32\com.hpp"

namespace isoeng::graphics::d3d12 {

class Fence {
public:
  enum class Error { NO, CREATE_FAILED, CREATE_EVENT_FAILED, NO_VALID_DEVICE_PROVIDED, NO_VALID_QUEUE_PROVIDED, SIGNAL_FAILED, SET_EVENT_FAILED };

  [[nodiscard]] static std::expected<Fence, Error> Create(const isoeng::pointer::win32::ComPtr<ID3D12Device> &d);

  [[nodiscard]] inline const isoeng::pointer::win32::ComPtr<ID3D12Fence> &Get() const noexcept { return _fence; }

  [[nodiscard]] Error Signal(const isoeng::pointer::win32::ComPtr<ID3D12CommandQueue> &queue) noexcept;
  [[nodiscard]] Error Wait() const noexcept;

private:
  static constexpr isoeng::log::Log _log{isoeng::log::log_lvl<isoeng::log::Trace::All>, isoeng::log::string<"engine.graphics.d3d12.fence">};

  const isoeng::pointer::win32::ComPtr<ID3D12Fence> _fence;
  const HANDLE _event;
  u64 _value;

  Fence(isoeng::pointer::win32::ComPtr<ID3D12Fence> &&f, const HANDLE e);
};

} // namespace isoeng::graphics::d3d12