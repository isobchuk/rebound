#pragma once

namespace isoeng::pointer::win32 {

// concept com_

template <typename T> class ComPtr final {
public:
  ComPtr(T *p) : _ptr(p) {}

  ~ComPtr() {
    if (_ptr) [[likely]] {
      _ptr->Release();
    }
  }

private:
  T *_ptr;
};

} // namespace isoeng::pointer::win32