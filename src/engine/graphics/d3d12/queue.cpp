#include "engine\graphics\d3d12\queue.hpp"

using namespace isoeng::graphics::d3d12;

std::expected<Queue, Queue::Error> Queue::Create(const isoeng::pointer::win32::ComPtr<ID3D12Device> &d) {
  using namespace isoeng::pointer::win32;
  using namespace isoeng::log;
  using enum Error;

  if (d) [[likely]] {
    const D3D12_COMMAND_QUEUE_DESC desc{.Type = D3D12_COMMAND_LIST_TYPE_DIRECT,
                                        .Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,
                                        .Flags = D3D12_COMMAND_QUEUE_FLAG_NONE,
                                        .NodeMask = 0U};

    ComPtr<ID3D12CommandQueue> queue;

    const auto hr = d.Get()->CreateCommandQueue(&desc, IID_PPV_ARGS(queue.Out()));
    if (FAILED(hr)) [[unlikely]] {
      _log.error(string<"Could not create command queue with error [%X]!">, hr);
      return std::unexpected(CREATE_FAILED);
    }

    _log.info(string<"Command queue was successfully created.">);
    return Queue(std::move(queue));
  }

  _log.error(string<"No valid device provided!">);
  return std::unexpected(NO_VALID_DEVICE_PROVIDED);
}

Queue::Queue(isoeng::pointer::win32::ComPtr<ID3D12CommandQueue> &&q) : _queue(std::move(q)) {}

Queue::Error Queue::Execute(const isoeng::pointer::win32::ComPtr<ID3D12GraphicsCommandList> &list) const noexcept {
  using namespace isoeng::log;
  using enum Error;

  if (!list) [[unlikely]] {
    _log.error(string<"No valid command list provided!">);
    return NO_VALID_COMMAND_LIST_PROVIDED;
  }

  ID3D12CommandList *const commandLists[] = {list.Get()};
  _log.info(string<"Execute [%u] command lists">, std::size(commandLists));
  _queue.Get()->ExecuteCommandLists(std::size(commandLists), commandLists);
  return NO;
}