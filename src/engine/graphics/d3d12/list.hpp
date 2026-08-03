#pragma once

#include <array>
#include <expected>

#include <d3d12.h>

#include "engine\framework\log\log.hpp"
#include "engine\platform\pointer\win32\com.hpp"

namespace isoeng::graphics::d3d12 {

class List {
public:
  enum class Error {
    NO,
    CREATE_FAILED,
    NO_VALID_DEVICE_PROVIDED,
    NO_VALID_COMMAND_ALLOCATOR_PROVIDED,
    NO_VALID_RESOURCE_PROVIDED,
    CLOSE_LIST_FAILED,
    RESET_FAILED,
    CLOSE_FAILED
  };

  struct ResourceState {
    const D3D12_RESOURCE_STATES before;
    const D3D12_RESOURCE_STATES after;

    constexpr ResourceState(const D3D12_RESOURCE_STATES b, const D3D12_RESOURCE_STATES a) : before(b), after(a) {}
  };

  using Color = std::array<f32, 4U>;

  [[nodiscard]] static std::expected<List, Error> Create(const isoeng::pointer::win32::ComPtr<ID3D12Device> &d,
                                                         const isoeng::pointer::win32::ComPtr<ID3D12CommandAllocator> &a);

  [[nodiscard]] inline const isoeng::pointer::win32::ComPtr<ID3D12GraphicsCommandList> &Get() const noexcept { return _list; }

  [[nodiscard]] Error Begin(const isoeng::pointer::win32::ComPtr<ID3D12CommandAllocator> &a) const noexcept;
  [[nodiscard]] Error Transition(const isoeng::pointer::win32::ComPtr<ID3D12Resource> &resource, const ResourceState state) const noexcept;
  void SetRenderTarget(const D3D12_CPU_DESCRIPTOR_HANDLE handle) const noexcept;
  void Clear(const D3D12_CPU_DESCRIPTOR_HANDLE handle, const Color &color) const noexcept;
  [[nodiscard]] Error End() const noexcept;

private:
  static constexpr isoeng::log::Log _log{isoeng::log::log_lvl<isoeng::log::Trace::All>, isoeng::log::string<"engine.graphics.d3d12.list">};

  const isoeng::pointer::win32::ComPtr<ID3D12GraphicsCommandList> _list;

  List(isoeng::pointer::win32::ComPtr<ID3D12GraphicsCommandList> &&l);
};

} // namespace isoeng::graphics::d3d12