#include "engine\graphics\d3d12\device.hpp"

using namespace isoeng::graphics::d3d12;

std::expected<Device, Device::Error> Device::Create(const isoeng::pointer::win32::ComPtr<IDXGIAdapter4> &a,
                                                    const D3D_FEATURE_LEVEL lvl /* = D3D_FEATURE_LEVEL_11_0*/) {
  using namespace isoeng::log;
  using enum Error;

  if (a) [[likely]] {
    isoeng::pointer::win32::ComPtr<ID3D12Device> device;
    const auto hr = D3D12CreateDevice(a.Get(), lvl, IID_PPV_ARGS(device.Out()));
    if (FAILED(hr)) [[unlikely]] {
      _log.error(string<"Could not create D3D12 device with error [%X]!">, hr);
      return std::unexpected(CREATE_FAILED);
    }

    _log.info(string<"D3D12 device was successfully created with D3D12 level [%X].">, static_cast<std::underlying_type_t<decltype(lvl)>>(lvl));
    return Device(std::move(device));
  }

  _log.error(string<"No valid adapter provided!">);
  return std::unexpected(NO_VALID_ADAPTER_PROVIDED);
}

Device::Device(isoeng::pointer::win32::ComPtr<ID3D12Device> &&d) : _device(std::move(d)) {}