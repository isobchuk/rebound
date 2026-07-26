#include "engine\graphics\d3d12\fence.hpp"

using namespace isoeng::graphics::d3d12;

std::expected<Fence, Fence::Error> Fence::Create(const isoeng::pointer::win32::ComPtr<ID3D12Device> &d) {
  using namespace isoeng::pointer::win32;
  using namespace isoeng::log;
  using enum Error;

  if (d) [[likely]] {
    ComPtr<ID3D12Fence> fence;

    const auto hr = d.Get()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fence.Out()));

    if (FAILED(hr)) [[unlikely]] {
      _log.error(string<"Could not create fence with error [%X]!">, hr);
      return std::unexpected(CREATE_FAILED);
    }

    _log.info(string<"Fence was successfully created.">);
    return Fence(std::move(fence));
  }

  _log.error(string<"No valid device provided!">);
  return std::unexpected(NO_VALID_DEVICE_PROVIDED);
}

Fence::Fence(isoeng::pointer::win32::ComPtr<ID3D12Fence> &&f) : _fence(std::move(f)) {}