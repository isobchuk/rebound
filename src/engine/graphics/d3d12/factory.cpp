#include "engine/graphics/d3d12/factory.hpp"

using namespace isoeng::graphics::d3d12;

Factory::Factory() { CreateDXGIFactory2(0U, IID_PPV_ARGS(&_factory)); }