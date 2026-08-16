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

    const HANDLE _event = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (nullptr == _event) [[unlikely]] {
      _log.error(string<"Could not create an event!">);
      return std::unexpected(CREATE_EVENT_FAILED);
    }

    _log.info(string<"Fence was successfully created.">);
    return Fence(std::move(fence), _event);
  }

  _log.error(string<"No valid device provided!">);
  return std::unexpected(NO_VALID_DEVICE_PROVIDED);
}

Fence::Fence(isoeng::pointer::win32::ComPtr<ID3D12Fence> &&f, const HANDLE e) : _fence(std::move(f)), _event(e), _value() {}

// TODO: return u64 value
Fence::Error Fence::Signal(const isoeng::pointer::win32::ComPtr<ID3D12CommandQueue> &queue) noexcept {
  using namespace isoeng::log;
  using enum Error;

  if (!queue) [[unlikely]] {
    _log.error(string<"No valid queue provided!">);
    return NO_VALID_QUEUE_PROVIDED;
  }

  const auto hr = queue.Get()->Signal(_fence.Get(), ++_value);
  if (FAILED(hr)) [[unlikely]] {
    _log.error(string<"Could not create fence with error [%X]!">, hr);
    return SIGNAL_FAILED;
  }

  return NO;
}

// TODO: Analyze and rework
Fence::Error Fence::Wait() const noexcept {
  using namespace isoeng::log;
  using enum Error;

  if (_fence.Get()->GetCompletedValue() <= _value) {
    const auto hr = _fence.Get()->SetEventOnCompletion(_value, _event);
    if (FAILED(hr)) [[unlikely]] {
      _log.error(string<"Could not create fence with error [%X]!">, hr);
      return SET_EVENT_FAILED;
    }

    if (WAIT_OBJECT_0 != WaitForSingleObject(_event, INFINITE)) {
      _log.error(string<"Wait object failed!">);
    }
  }
  return NO;
}
