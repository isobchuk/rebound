#include "engine\graphics\d3d12\list.hpp"

using namespace isoeng::graphics::d3d12;

std::expected<List, List::Error> List::Create(const isoeng::pointer::win32::ComPtr<ID3D12Device> &d,
                                              const isoeng::pointer::win32::ComPtr<ID3D12CommandAllocator> &a) {
  using namespace isoeng::pointer::win32;
  using namespace isoeng::log;
  using enum Error;

  if (!d) [[unlikely]] {
    _log.error(string<"No valid device provided!">);
    return std::unexpected(NO_VALID_DEVICE_PROVIDED);
  }

  if (!a) [[unlikely]] {
    _log.error(string<"No valid command queue provided!">);
    return std::unexpected(NO_VALID_COMMAND_ALLOCATOR_PROVIDED);
  }

  ComPtr<ID3D12GraphicsCommandList> list;

  auto hr = d.Get()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, a.Get(), nullptr, IID_PPV_ARGS(list.Out()));
  if (FAILED(hr)) [[unlikely]] {
    _log.error(string<"Could not create command list with error [%X]!">, hr);
    return std::unexpected(CREATE_FAILED);
  }

  hr = list.Get()->Close();
  if (FAILED(hr)) [[unlikely]] {
    _log.error(string<"Close list failed with error [%X]!">, hr);
    return std::unexpected(CLOSE_LIST_FAILED);
  }

  _log.info(string<"Command list was successfully created.">);
  return List(std::move(list));
}

List::List(isoeng::pointer::win32::ComPtr<ID3D12GraphicsCommandList> &&l) : _list(std::move(l)) {}