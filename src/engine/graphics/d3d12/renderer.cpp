#include "engine/graphics/d3d12/renderer.hpp"

using namespace isoeng::graphics::d3d12;

Renderer::Renderer() : _factory(), _adapter(&_factory.Get()), _device() {}