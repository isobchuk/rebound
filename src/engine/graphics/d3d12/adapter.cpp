#include <algorithm>
#include <vector>

#include <d3d12.h>

#include "engine/graphics/d3d12/adapter.hpp"

using namespace isoeng::graphics::d3d12;

std::expected<Adapter, Adapter::Error> Adapter::Create(const isoeng::pointer::win32::ComPtr<IDXGIFactory4> &f) {
  using namespace isoeng::log;
  using enum Error;

  if (f) [[likely]] {
    std::vector<AdapterInfo> adapters;
    isoeng::pointer::win32::ComPtr<IDXGIAdapter1> adapter1;

    // Enum adapters
    for (u32 i = 0U; DXGI_ERROR_NOT_FOUND != f.Get()->EnumAdapters1(i, adapter1.Out()); i++) {
      DXGI_ADAPTER_DESC1 descriptor;
      adapter1.Get()->GetDesc1(&descriptor);

      // If it is HW adapter - push it in the vector (support only HW adapters for now)
      if (!(descriptor.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)) {
        adapters.push_back(AdapterInfo(descriptor.Description, descriptor.DedicatedVideoMemory, std::move(adapter1)));
        _log.info(string<"Found HW adapter %u: %s with dedicated video memory %u bytes.">, i, adapters.back().name, adapters.back().memory);
      }
    }

    // Sort with memory value (bigger memory - better)
    std::sort(adapters.begin(), adapters.end(), [](const AdapterInfo &lhs, const AdapterInfo &rhs) { return lhs.memory > rhs.memory; });

    // Check if we can create device with a given adapter by priority
    for (auto &info : adapters) {
      if (SUCCEEDED(D3D12CreateDevice(info.adapter1.Get(), D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr))) {
        _log.info(string<"Choosen adapter: %s">, ASCIIString(info.name));
        isoeng::pointer::win32::ComPtr<IDXGIAdapter4> adapter4;

        // Try to convert, if can not - continue with other
        if (FAILED(info.adapter1.As(adapter4))) {
          _log.error(string<"Could not convert adapter1 to adapter4!">);
          continue;
        }

        // Choose adapter, exit
        return Adapter(std::move(adapter4), std::move(info));
      }
    }

    _log.error(string<"No appropriate adapter found!">);
    return std::unexpected(ADAPTER_NOT_FOUND);
  }

  _log.error(string<"No valid factory provided!">);
  return std::unexpected(NO_VALID_FACTORY_PROVIDED);
}

Adapter::Adapter(isoeng::pointer::win32::ComPtr<IDXGIAdapter4> &&a, const AdapterInfo &&info) : _adapter(std::move(a)), _info(std::move(info)) {}