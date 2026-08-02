#include "engine\graphics\d3d12\allocator.hpp"

using namespace isoeng::graphics::d3d12;

std::expected<Allocator, Allocator::Error> Allocator::Create(const isoeng::pointer::win32::ComPtr<ID3D12Device> &d) {
  using namespace isoeng::pointer::win32;
  using namespace isoeng::log;
  using enum Error;

  if (d) [[likely]] {
    ComPtr<ID3D12CommandAllocator> allocator;

    const auto hr = d.Get()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(allocator.Out()));
    if (FAILED(hr)) [[unlikely]] {
      _log.error(string<"Could not create command allocator with error [%X]!">, hr);
      return std::unexpected(CREATE_FAILED);
    }

    _log.info(string<"Command allocator was successfully created.">);
    return Allocator(std::move(allocator));
  }

  _log.error(string<"No valid device provided!">);
  return std::unexpected(NO_VALID_DEVICE_PROVIDED);
}

Allocator::Allocator(isoeng::pointer::win32::ComPtr<ID3D12CommandAllocator> &&a) : _allocator(std::move(a)) {}