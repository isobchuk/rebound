#pragma once

#include <expected>

#include <d3d12.h>

#include "engine\framework\log\log.hpp"
#include "engine\platform\pointer\win32\com.hpp"

namespace isoeng::graphics::d3d12 {

class Queue {
public:
  enum class Error { NO, CREATE_FAILED, NO_VALID_DEVICE_PROVIDED, NO_VALID_COMMAND_LIST_PROVIDED };

  [[nodiscard]] static std::expected<Queue, Error> Create(const isoeng::pointer::win32::ComPtr<ID3D12Device> &d);

  [[nodiscard]] inline const isoeng::pointer::win32::ComPtr<ID3D12CommandQueue> &Get() const noexcept { return _queue; }

  [[nodiscard]] Error Execute(const isoeng::pointer::win32::ComPtr<ID3D12GraphicsCommandList> &list) const noexcept;

private:
  static constexpr isoeng::log::Log _log{isoeng::log::log_lvl<isoeng::log::Trace::All>, isoeng::log::string<"engine.graphics.d3d12.queue">};

  const isoeng::pointer::win32::ComPtr<ID3D12CommandQueue> _queue;

  Queue(isoeng::pointer::win32::ComPtr<ID3D12CommandQueue> &&q);
};

} // namespace isoeng::graphics::d3d12