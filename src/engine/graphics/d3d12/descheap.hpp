#pragma once

#include <cassert>
#include <expected>

#include <d3d12.h>

#include "engine\framework\log\log.hpp"
#include "engine\platform\pointer\win32\com.hpp"

namespace isoeng::graphics::d3d12 {

template <const D3D12_DESCRIPTOR_HEAP_TYPE type, const bool visible = false>
concept SupportsGpuHandle = visible && type != D3D12_DESCRIPTOR_HEAP_TYPE_RTV && type != D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

template <const D3D12_DESCRIPTOR_HEAP_TYPE type, const u32 count,
          const bool visible = ((D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV == type) || (D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER == type))>
class DescriptorHeap {
public:
  enum class Error { NO, CREATE_FAILED, NO_VALID_DEVICE_PROVIDED };

  [[nodiscard]] static std::expected<DescriptorHeap, Error> Create(const isoeng::pointer::win32::ComPtr<ID3D12Device> &d);

  [[nodiscard]] inline const isoeng::pointer::win32::ComPtr<ID3D12DescriptorHeap> &Get() const noexcept { return _heap; }

  [[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE CpuHandle(const u32 index) const noexcept;
  [[nodiscard]] D3D12_GPU_DESCRIPTOR_HANDLE GpuHandle(u32 index) const noexcept
  requires SupportsGpuHandle<type, visible>;

private:
  static_assert(((type == D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) || (type == D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER) ||
                 (type == D3D12_DESCRIPTOR_HEAP_TYPE_RTV) || (type == D3D12_DESCRIPTOR_HEAP_TYPE_DSV)),
                "Type should be one of the valid enum number!");
  static_assert((0U != count), "Count should be bigger than 0!");

  static constexpr isoeng::log::Log _log{isoeng::log::log_lvl<isoeng::log::Trace::All>, isoeng::log::string<"engine.graphics.d3d12.heap">};

  static constexpr auto _TYPE = type;
  static constexpr auto _COUNT = count;
  static constexpr auto _VISIBLE = visible;

  const isoeng::pointer::win32::ComPtr<ID3D12DescriptorHeap> _heap;
  const u32 _size;

  DescriptorHeap(isoeng::pointer::win32::ComPtr<ID3D12DescriptorHeap> &&dh, const u32 sz);
};

template <const u32 count, const bool visible = false> using RTVHeapDescriptor = DescriptorHeap<D3D12_DESCRIPTOR_HEAP_TYPE_RTV, count, visible>;
template <const u32 count, const bool visible = true>
using CBVSRVUAVHeapDescriptor = DescriptorHeap<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, count, visible>;

template <const D3D12_DESCRIPTOR_HEAP_TYPE type, const u32 count, const bool visible>
std::expected<DescriptorHeap<type, count, visible>, typename DescriptorHeap<type, count, visible>::Error>
DescriptorHeap<type, count, visible>::Create(const isoeng::pointer::win32::ComPtr<ID3D12Device> &d) {
  using namespace isoeng::pointer::win32;
  using namespace isoeng::log;

  if (d) [[likely]] {
    const D3D12_DESCRIPTOR_HEAP_DESC descriptor{.Type = _TYPE,
                                                .NumDescriptors = _COUNT,
                                                .Flags = _VISIBLE ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
                                                .NodeMask = 0U};

    isoeng::pointer::win32::ComPtr<ID3D12DescriptorHeap> heap;
    const auto hr = d.Get()->CreateDescriptorHeap(&descriptor, IID_PPV_ARGS(heap.Out()));
    if (FAILED(hr)) [[unlikely]] {
      _log.error(string<"Could not create descriptor heap device with error [%X]! (Type [%d] Count [%u] Visible [%b])">, hr,
                 static_cast<std::underlying_type_t<decltype(_TYPE)>>(_TYPE), _COUNT, _VISIBLE);
      return std::unexpected(Error::CREATE_FAILED);
    }

    const auto size = d.Get()->GetDescriptorHandleIncrementSize(_TYPE);

    _log.info(string<"Descriptor Heap was successfully created.">);
    return DescriptorHeap(std::move(heap), size);
  }

  _log.error(string<"No valid device provided!">);
  return std::unexpected(Error::NO_VALID_DEVICE_PROVIDED);
}

template <const D3D12_DESCRIPTOR_HEAP_TYPE type, const u32 count, const bool visible>
DescriptorHeap<type, count, visible>::DescriptorHeap(isoeng::pointer::win32::ComPtr<ID3D12DescriptorHeap> &&dh, const u32 sz)
    : _heap(std::move(dh)), _size(sz) {}

template <const D3D12_DESCRIPTOR_HEAP_TYPE type, const u32 count, const bool visible>
D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeap<type, count, visible>::CpuHandle(const u32 index) const noexcept {
  assert(index < _COUNT);
  auto handle = _heap.Get()->GetCPUDescriptorHandleForHeapStart();
  handle.ptr += index * _size;
  return handle;
}

template <const D3D12_DESCRIPTOR_HEAP_TYPE type, const u32 count, const bool visible>
D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeap<type, count, visible>::GpuHandle(const u32 index) const noexcept
requires SupportsGpuHandle<type, visible>
{
  assert(index < _COUNT);
  auto handle = _heap.Get()->GetGPUDescriptorHandleForHeapStart();
  handle.ptr += index * _size;
  return handle;
}

} // namespace isoeng::graphics::d3d12