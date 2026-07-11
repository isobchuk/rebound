#pragma once

#include <source_location>

#include "engine/framework/log/format.hpp"

// Basic namespace to work with log component
namespace isoeng::log {

/**
 * @brief     Concept to check the provided type has tick() method (should return the time from launch in ms (unsigned integral))
 *
 * @tparam T  The type should be checked
 */
template <typename T>
concept time_func = requires(T &tick) {
  { tick.tick() } -> std::unsigned_integral;
};

/**
 * @brief     Concept to check that type is appropriate to isoeng::log::put and time_func concepts
 *
 * @tparam T  The type should be checked
 */
template <typename T>
concept time = isoeng::log::put<T> && time_func<T>;

/**
 * @brief Enumeration of the trace levels
 *
 */
enum class Trace : unsigned {
  All,

  Trace,
  Debug,
  Info,
  Warn,
  Error,
  Fatal,

  None
};

// Trace highlighting
enum class Highlight { Disabled, Enabled };

// Colours to display in the terminal according to the level
template <const Highlight col> struct TraceHighlight {
  static constexpr char def[] = "";
  static constexpr char cyan[] = "";
  static constexpr char red[] = "";
  static constexpr char yellow[] = "";
};

// Overload for the enabled highlight
template <> struct TraceHighlight<Highlight::Enabled> {
  static constexpr char def[] = "\u001b[0m";
  static constexpr char cyan[] = "\u001b[36m";
  static constexpr char red[] = "\u001b[31m";
  static constexpr char yellow[] = "\u001b[33m";
};

// Transform trace level to type property
template <const Trace lvl, const Highlight col = Highlight::Disabled> struct TraceLevel final {
  static constexpr Trace level = lvl;
  static constexpr Highlight colour = col;
  struct TraceLevelT;
};

// Inline variable to use outside
template <const Trace lvl, const Highlight col = Highlight::Disabled> inline constexpr auto log_lvl = TraceLevel<lvl, col>{};

// Check if type is TraceLevel
template <typename T>
concept log_level = requires(T) {
  T::level;
  T::colour;
  typename T::TraceLevelT;
};

/**
 * @brief               Compile-time class to log information
 *
 * @tparam Output       The type that should be satisfied to time concept
 * @tparam LogLevel     The type that should be satisfied to log_level concept (all messages below the provided log_level will be ignored)
 * @tparam Component    The type that should be satisfied to isoeng::log::const_string concept
 */
template <time Output, log_level LogLevel = TraceLevel<Trace::All>, isoeng::log::const_string Component = decltype(isoeng::log::string<"">)>
class Log final {
  const Output &out;                                             // Reference to the Output object
  static constexpr Trace level = LogLevel::level;                // Logging level that has been requested
  static constexpr auto component = Component{};                 // Name of the component
  static constexpr TraceHighlight<LogLevel::colour> highlight{}; // Trace highlight

public:
  const isoeng::log::Format<Output> format; // The compile time format object (just to provide access if needed)

  /**
   * @brief           Compile-time constructors with the only one mandatory parameter
   *
   * @param o         Reference to the Output class object
   * @param LogLevel  Level of tracing, the functions for level lower than provided transform to empty
   * @param Component Name of the component
   *
   * @example         static constexpr isoeng::log::Log debug{debugPuts, format::string<"UART">};
   * @example         static constexpr isoeng::log::Log debug{debugPuts, log::log_lvl<log::Trace::Warn>, format::string<"GLOBAL">};
   */
  consteval Log(const Output &o) : out(o), format(o) {}
  consteval Log(const Output &o, const LogLevel = LogLevel{}) : out(o), format(o) {}
  consteval Log(const Output &o, const Component = Component{}) : out(o), format(o) {}
  consteval Log(const Output &o, const LogLevel = LogLevel{}, const Component = Component{}) : out(o), format(o) {}

  /**
   * @brief         Function to print line with time mark
   *
   * @param S       Compile time string string with the specifiers to be formatted
   * @param args    Variables that should be formatted and placed inside string
   */
  template <isoeng::log::const_string S, typename... Args> inline void message(const S, const Args... args) const {
    using namespace isoeng::log;
    constexpr auto res_str = string<"[%t] [MESSAGE] "> + string<"["> + component + string<"]"> + string<": "> + string<S::string>;
    format.println(res_str, out.tick(), args...);
  }

  /**
   * @brief             Function to print buffer with time mark
   *
   * @tparam S          Compile time string string with the specifiers to be formatted
   * @param dataBuffer  Object with dynamic data
   * @param args        Variables that should be formatted and placed inside string
   */
  template <isoeng::log::const_string S, typename... Args>
  inline void message(const S, const isoeng::log::DataBuffer &dataBuffer, const Args... args) const {
    using namespace isoeng::log;
    constexpr auto res_str = string<"[%t] [MESSAGE] "> + string<"["> + component + string<"]"> + string<": "> + string<S::string>;
    format.printf(res_str, dataBuffer, out.tick(), args...);
  }

  /**
   * @brief         Function to print fatal message with time mark
   *
   * @param S       Compile time string string with the specifiers to be formatted
   * @param args    Variables that should be formatted and placed inside string
   */
  template <isoeng::log::const_string S, typename... Args> inline void fatal(const S, const Args... args) const {
    using namespace isoeng::log;
    if constexpr (Trace::Fatal >= level) {
      constexpr auto res_str = string<"[%t] [FATAL] "> + string<"["> + component + string<"]"> + string<": "> + string<S::string>;
      format.println(res_str, out.tick(), args...);
    }
  }

  /**
   * @brief             Function to print fatal buffer with time mark
   *
   * @tparam S          Compile time string string with the specifiers to be formatted
   * @param dataBuffer  Object with dynamic data
   * @param args        Variables that should be formatted and placed inside string
   */
  template <isoeng::log::const_string S, typename... Args>
  inline void fatal(const S, const isoeng::log::DataBuffer &dataBuffer, const Args... args) const {
    using namespace isoeng::log;
    if constexpr (Trace::Fatal >= level) {
      constexpr auto res_str = string<"[%t] [FATAL] "> + string<"["> + component + string<"]"> + string<": "> + string<S::string>;
      format.printf(res_str, dataBuffer, out.tick(), args...);
    }
  }

  /**
   * @brief         Function to print error message with time mark
   *
   * @param S       Compile time string string with the specifiers to be formatted
   * @param args    Variables that should be formatted and placed inside string
   */
  template <isoeng::log::const_string S, typename... Args> inline void error(const S, const Args... args) const {
    using namespace isoeng::log;
    if constexpr (Trace::Error >= level) {
      constexpr auto res_str = string<"[%t] [ERROR] "> + string<"["> + component + string<"]"> + string<": "> + string<S::string>;
      format.println(res_str, out.tick(), args...);
    }
  }

  /**
   * @brief             Function to print error buffer with time mark
   *
   * @tparam S          Compile time string string with the specifiers to be formatted
   * @param dataBuffer  Object with dynamic data
   * @param args        Variables that should be formatted and placed inside string
   */
  template <isoeng::log::const_string S, typename... Args>
  inline void error(const S, const isoeng::log::DataBuffer &dataBuffer, const Args... args) const {
    using namespace isoeng::log;
    if constexpr (Trace::Error >= level) {
      constexpr auto res_str = string<"[%t] [ERROR] "> + string<"["> + component + string<"]"> + string<": "> + string<S::string>;
      format.printf(res_str, dataBuffer, out.tick(), args...);
    }
  }

  /**
   * @brief         Function to print warning message with time mark
   *
   * @param S       Compile time string string with the specifiers to be formatted
   * @param args    Variables that should be formatted and placed inside string
   */
  template <isoeng::log::const_string S, typename... Args> inline void warning(const S, const Args... args) const {
    using namespace isoeng::log;
    if constexpr (Trace::Warn >= level) {
      constexpr auto res_str = string<"[%t] [WARN] "> + string<"["> + component + string<"]"> + string<": "> + string<S::string>;
      format.println(res_str, out.tick(), args...);
    }
  }

  /**
   * @brief             Function to print warning buffer with time mark
   *
   * @tparam S          Compile time string string with the specifiers to be formatted
   * @param dataBuffer  Object with dynamic data
   * @param args        Variables that should be formatted and placed inside string
   */
  template <isoeng::log::const_string S, typename... Args>
  inline void warning(const S, const isoeng::log::DataBuffer &dataBuffer, const Args... args) const {
    using namespace isoeng::log;
    if constexpr (Trace::Warn >= level) {
      constexpr auto res_str = string<"[%t] [WARN] "> + string<"["> + component + string<"]"> + string<": "> + string<S::string>;
      format.printf(res_str, dataBuffer, out.tick(), args...);
    }
  }

  /**
   * @brief         Function to print info message with time mark
   *
   * @param S       Compile time string string with the specifiers to be formatted
   * @param args    Variables that should be formatted and placed inside string
   */
  template <isoeng::log::const_string S, typename... Args> inline void info(const S, const Args... args) const {
    using namespace isoeng::log;
    if constexpr (Trace::Info >= level) {
      constexpr auto res_str = string<"[%t] [INFO] "> + string<"["> + component + string<"]"> + string<": "> + string<S::string>;
      format.println(res_str, out.tick(), args...);
    }
  }

  /**
   * @brief             Function to print info buffer with time mark
   *
   * @tparam S          Compile time string string with the specifiers to be formatted
   * @param dataBuffer  Object with dynamic data
   * @param args        Variables that should be formatted and placed inside string
   */
  template <isoeng::log::const_string S, typename... Args>
  inline void info(const S, const isoeng::log::DataBuffer &dataBuffer, const Args... args) const {
    using namespace isoeng::log;
    if constexpr (Trace::Info >= level) {
      constexpr auto res_str = string<"[%t] [INFO] "> + string<"["> + component + string<"]"> + string<": "> + string<S::string>;
      format.printf(res_str, dataBuffer, out.tick(), args...);
    }
  }

  /**
   * @brief         Function to print debug message with time mark
   *
   * @param S       Compile time string string with the specifiers to be formatted
   * @param args    Variables that should be formatted and placed inside string
   */
  template <isoeng::log::const_string S, typename... Args> inline void debug(const S, const Args... args) const {
    using namespace isoeng::log;
    if constexpr (Trace::Debug >= level) {
      constexpr auto res_str = string<"[%t] [DEBUG] "> + string<"["> + component + string<"]"> + string<": "> + string<S::string>;
      format.println(res_str, out.tick(), args...);
    }
  }

  /**
   * @brief             Function to print debug buffer with time mark
   *
   * @tparam S          Compile time string string with the specifiers to be formatted
   * @param dataBuffer  Object with dynamic data
   * @param args        Variables that should be formatted and placed inside string
   */
  template <isoeng::log::const_string S, typename... Args>
  inline void debug(const S, const isoeng::log::DataBuffer &dataBuffer, const Args... args) const {
    using namespace isoeng::log;
    if constexpr (Trace::Debug >= level) {
      constexpr auto res_str = string<"[%t] [DEBUG] "> + string<"["> + component + string<"]"> + string<": "> + string<S::string>;
      format.printf(res_str, dataBuffer, out.tick(), args...);
    }
  }

  /**
   * @brief         Function to print trace message with time mark
   *
   * @param S       Compile time string string with the specifiers to be formatted
   * @param args    Variables that should be formatted and placed inside string
   */
  template <isoeng::log::const_string S, typename... Args> inline void trace(const S, const Args... args) const {
    using namespace isoeng::log;
    if constexpr (Trace::Trace >= level) {
      constexpr auto res_str = string<"[%t] [TRACE] "> + string<"["> + component + string<"]"> + string<": "> + string<S::string>;
      format.println(res_str, out.tick(), args...);
    }
  }

  /**
   * @brief             Function to print trace buffer with time mark
   *
   * @tparam S          Compile time string string with the specifiers to be formatted
   * @param dataBuffer  Object with dynamic data
   * @param args        Variables that should be formatted and placed inside string
   */
  template <isoeng::log::const_string S, typename... Args>
  inline void trace(const S, const isoeng::log::DataBuffer &dataBuffer, const Args... args) const {
    using namespace isoeng::log;
    if constexpr (Trace::Trace >= level) {
      constexpr auto res_str = string<"[%t] [TRACE] "> + string<"["> + component + string<"]"> + string<": "> + string<S::string>;
      format.printf(res_str, dataBuffer, out.tick(), args...);
    }
  }
};

} // namespace isoeng::log