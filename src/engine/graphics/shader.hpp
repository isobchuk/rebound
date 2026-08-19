#pragma once

#include <expected>
#include <fstream>
#include <vector>

#include "engine\framework\log\log.hpp"

namespace isoeng::graphics {
class Shader final {
public:
  enum class Type { VERTEX, PIXEL };

  template <isoeng::log::const_string auto dir, Type type> class File final {
  private:
    static constexpr const char _ENDINGS[][3] = {"vs", "ps"};

  public:
    template <isoeng::log::const_string Name>
    [[nodiscard]] static consteval auto Path(const Name name) noexcept
        -> decltype(dir + isoeng::log::string<"/"> + name + isoeng::log::string<"."> +
                    isoeng::log::string<_ENDINGS[static_cast<std::underlying_type_t<decltype(type)>>(type)]> + isoeng::log::string<".cso">) {
      return {};
    }
  };

  enum class Error { NO, OPEN_FILE_FAILED, GET_FILE_SIZE_FAILED, READ_FILE_FAILED };

  using Bytecode = std::vector<isoeng::byte>;

  template <isoeng::log::const_string Path> [[nodiscard]] static std::expected<Shader, Error> Create(const Path) noexcept {
    using namespace isoeng::log;
    using enum Error;

    constexpr Path path{};
    _log.info(string<"Load shaders from a path %S">, path);

    std::ifstream file(path.c_str(), std::ios::binary | std::ios::ate);
    if (!file) [[unlikely]] {
      _log.error(string<"Could not open shader file!">);
      return std::unexpected(OPEN_FILE_FAILED);
    }

    const auto size = static_cast<usize>(file.tellg());
    if (size < 0) [[unlikely]] {
      _log.error(string<"Could not get file size with error [%u]!">, size);
      return std::unexpected(GET_FILE_SIZE_FAILED);
    }

    Bytecode bytecode(size);

    file.seekg(0);

    if (!file.read(static_cast<char *>(static_cast<void *>(bytecode.data())), static_cast<std::streamsize>(bytecode.size()))) {
      _log.error(string<"Could not get read shader file!">);
      return std::unexpected(READ_FILE_FAILED);
    }

    return Shader(std::move(bytecode));
  }

  [[nodiscard]] inline const Bytecode &Get() const noexcept { return _bytecode; }

private:
  static constexpr isoeng::log::Log _log{isoeng::log::log_lvl<isoeng::log::Trace::All>, isoeng::log::string<"engine.graphics.shader">};

  const Bytecode _bytecode;

  Shader(Bytecode &&bc) : _bytecode(std::move(bc)) {}
};
} // namespace isoeng::graphics