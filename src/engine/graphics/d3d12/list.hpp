#pragma once

#include <expected>

#include <d3d12.h>

#include "engine\framework\log\log.hpp"
#include "engine\platform\pointer\win32\com.hpp"

namespace isoeng::graphics::d3d12 {

class List {
public:
  enum class Error { NO, CREATE_FAILED, NO_VALID_DEVICE_PROVIDED, NO_VALID_COMMAND_ALLOCATOR_PROVIDED, CLOSE_LIST_FAILED };

  [[nodiscard]] static std::expected<List, Error> Create(const isoeng::pointer::win32::ComPtr<ID3D12Device> &d,
                                                         const isoeng::pointer::win32::ComPtr<ID3D12CommandAllocator> &a);

  [[nodiscard]] inline const isoeng::pointer::win32::ComPtr<ID3D12GraphicsCommandList> &Get() const noexcept { return _list; }

private:
  static constexpr isoeng::log::Log _log{isoeng::log::log_lvl<isoeng::log::Trace::All>, isoeng::log::string<"engine.graphics.d3d12.list">};

  const isoeng::pointer::win32::ComPtr<ID3D12GraphicsCommandList> _list;

  List(isoeng::pointer::win32::ComPtr<ID3D12GraphicsCommandList> &&l);
};

} // namespace isoeng::graphics::d3d12