#pragma once

namespace isoeng::pointer::win32 {

// concept com_

template <typename T> class ComPtr final {
public:
  constexpr ComPtr() noexcept : _ptr() {}
  constexpr ComPtr(std::nullptr_t) noexcept : _ptr() {}

  ~ComPtr() {
    if (_ptr) [[likely]] {
      _ptr->Release();
    }
  }

  ComPtr(T *other) : _ptr(other) {
    if (_ptr) [[likely]] {
      _ptr->AddRef();
    }
  }

  ComPtr(const ComPtr &other) : _ptr(other._ptr) {
    if (_ptr) [[likely]] {
      _ptr->AddRef();
    }
  }

  ComPtr &operator=(const ComPtr &other) {
    if (this != &other) [[likely]] {
      if (_ptr) [[likely]] {
        _ptr->Release();
      }
      _ptr = other._ptr;
      if (_ptr) [[likely]] {
        _ptr->AddRef();
      }
    }
    return *this;
  }

  ComPtr(ComPtr &&other) : _ptr(other._ptr) { other._ptr = nullptr; }

  ComPtr &operator=(ComPtr &&other) {
    if (this != &other) [[likely]] {
      if (_ptr) [[likely]] {
        _ptr->Release();
      }
      _ptr = other._ptr;
      other._ptr = nullptr;
    }
    return *this;
  }

  //[[nodiscard]] inline T *const *operator&() const noexcept { return &_ptr; }
  [[nodiscard]] inline T **operator&() noexcept { return &_ptr; }

  [[nodiscard]] inline T *operator->() const noexcept { return _ptr; }
  [[nodiscard]] inline explicit operator bool() const noexcept { return _ptr != nullptr; }

private:
  T *_ptr;
};

} // namespace isoeng::pointer::win32