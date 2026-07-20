#include "engine/graphics/d3d12/adapter.hpp"

using namespace isoeng::graphics::d3d12;

std::expected<Adapter, Adapter::Error> Adapter::Create(const isoeng::pointer::win32::ComPtr<IDXGIFactory4> &f) {
  using namespace isoeng::log;

  if (f) [[likely]] {
    isoeng::pointer::win32::ComPtr<IDXGIAdapter1> adapter1;
    // usize memory = 0ULL;

    for (u32 i = 0U; DXGI_ERROR_NOT_FOUND != f->EnumAdapters1(i, &adapter1); i++) {
      DXGI_ADAPTER_DESC1 descriptor;
      adapter1->GetDesc1(&descriptor);

      const ASCIIString name(descriptor.Description);
      _log.info(string<"Found adapter %u: %s with dedicated video memory %u bytes.">, i, name, descriptor.DedicatedVideoMemory);

      /*if (_memory < descriptor.DedicatedVideoMemory) {
        // adapter1(&_adapter);
        _name = _memory = descriptor.DedicatedVideoMemory;
        _number = i;
      }*/
    }

    //_log.info(string<"Adapter choosen: ">, i, name, descriptor.DedicatedVideoMemory);

  } else {
    _log.error(string<"No valid factory provided!">);
  }

  return Adapter();
}

Adapter::Adapter() : _adapter() /*, _name(), _memory(), _number()*/ {}