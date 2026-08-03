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
    _log.error(string<"No valid command allocator provided!">);
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

List::Error List::Begin(const isoeng::pointer::win32::ComPtr<ID3D12CommandAllocator> &allocator) const noexcept {
  using namespace isoeng::log;
  using enum Error;

  if (!allocator) [[unlikely]] {
    _log.error(string<"No valid command allocator provided!">);
    return NO_VALID_COMMAND_ALLOCATOR_PROVIDED;
  }

  auto hr = allocator.Get()->Reset();
  if (FAILED(hr)) [[unlikely]] {
    _log.error(string<"Reset command allocator failed with error [%X]!">, hr);
    return RESET_FAILED;
  }

  hr = _list.Get()->Reset(allocator.Get(), nullptr);
  if (FAILED(hr)) [[unlikely]] {
    _log.error(string<"Reset command list failed with error [%X]!">, hr);
    return RESET_FAILED;
  }

  _log.info(string<"Command allocator and list were successfully reset.">);
  return NO;
}

List::Error List::Transition(const isoeng::pointer::win32::ComPtr<ID3D12Resource> &resource, const ResourceState state) const noexcept {
  using namespace isoeng::log;
  using enum Error;

  if (!resource) [[unlikely]] {
    _log.error(string<"No valid resourse provided!">);
    return NO_VALID_RESOURCE_PROVIDED;
  }

  const D3D12_RESOURCE_BARRIER barrier{.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
                                       .Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE,
                                       .Transition = {.pResource = resource.Get(),
                                                      .Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
                                                      .StateBefore = state.before,
                                                      .StateAfter = state.after}};

  _list.Get()->ResourceBarrier(1U, &barrier);
  _log.info(string<"Transition from state [%X] to state [%X].">, static_cast<std::underlying_type_t<decltype(state.before)>>(state.before),
            static_cast<std::underlying_type_t<decltype(state.after)>>(state.after));
  return NO;
}

void List::SetRenderTarget(const D3D12_CPU_DESCRIPTOR_HANDLE handle) const noexcept {
  using namespace isoeng::log;
  using enum Error;

  if (handle.ptr) [[likely]] {
    _list.Get()->OMSetRenderTargets(1U, &handle, FALSE, nullptr);
    _log.info(string<"Set Render target for handle [%X]">, handle.ptr);
  } else {
    _log.error(string<"Handle is NULL!">);
  }
}

void List::Clear(const D3D12_CPU_DESCRIPTOR_HANDLE handle, const List::Color &color) const noexcept {
  using namespace isoeng::log;
  using enum Error;

  if (handle.ptr) [[likely]] {
    _list.Get()->ClearRenderTargetView(handle, color.data(), 0, nullptr);
    _log.info(string<"Clear Render target View for handle [%X]">, handle.ptr);
  } else {
    _log.error(string<"Handle is NULL!">);
  }
}

List::Error List::End() const noexcept {
  using namespace isoeng::log;
  using enum Error;

  const auto hr = _list.Get()->Close();
  if (FAILED(hr)) [[unlikely]] {
    _log.error(string<"Close command list failed with error [%X]!">, hr);
    return CLOSE_FAILED;
  }

  return NO;
}