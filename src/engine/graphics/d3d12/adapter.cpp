#include "engine/graphics/d3d12/adapter.hpp"

using namespace isoeng::graphics::d3d12;

Adapter::Adapter(IDXGIFactory4 **factory) {
  using namespace isoeng::log;

  if (factory) [[likely]] {
    isoeng::pointer::win32::ComPtr<IDXGIAdapter1> adapter1;

    for (u32 i = 0U; DXGI_ERROR_NOT_FOUND != (*factory)->EnumAdapters1(i, &adapter1); i++) {
      DXGI_ADAPTER_DESC1 descriptor;
      adapter1->GetDesc1(&descriptor);

      _log.info(string<"Found adapter %u with dedicated video memory %u">, i, descriptor.DedicatedVideoMemory);
    }
  } else {
    _log.error(string<"No valid factory provided">);
  }
}