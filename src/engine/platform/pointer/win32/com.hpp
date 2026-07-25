#pragma once

#include <concepts>

#include <guiddef.h>
#include <winnt.h>

namespace isoeng::pointer::win32 {

template <typename T>
concept IUnknownInterface = requires(T &comObject, void *p) {
  { comObject.AddRef() } -> std::convertible_to<u64>;
  { comObject.Release() } -> std::convertible_to<u64>;
  { comObject.QueryInterface(GUID{}, &p) } -> std::convertible_to<HRESULT>;
};

template <IUnknownInterface T> class ComPtr final {
public:
  constexpr ComPtr() noexcept : _ptr() {}
  constexpr ComPtr(std::nullptr_t) noexcept : _ptr() {}

  ~ComPtr() { Reset(); }

  explicit ComPtr(T *other) : _ptr(other) {
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
      Reset();
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
      Reset();

      _ptr = other._ptr;
      other._ptr = nullptr;
    }
    return *this;
  }

  [[nodiscard]] inline T *const *Out() const noexcept { return &_ptr; }
  [[nodiscard]] inline T **Out() noexcept { return &_ptr; }

  [[nodiscard]] inline T *const Get() const noexcept { return _ptr; }
  [[nodiscard]] inline T *Get() noexcept { return _ptr; }

  [[nodiscard]] inline explicit operator bool() const noexcept { return _ptr != nullptr; }

  template <typename U> [[nodiscard]] inline HRESULT As(ComPtr<U> &out) const {
    out.Reset();
    return _ptr->QueryInterface(__uuidof(U), reinterpret_cast<void **>(out.Out()));
  }

  void Reset() noexcept {
    if (_ptr) [[likely]] {
      _ptr->Release();
      _ptr = nullptr;
    }
  }

private:
  T *_ptr;
};

} // namespace isoeng::pointer::win32