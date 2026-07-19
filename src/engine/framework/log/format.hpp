#pragma once

#include "engine/framework/log/ostream.hpp"
#include "engine/framework/log/string.hpp"

namespace isoeng::log {

/**
 * @brief Type that contains some dynamic buffer to print (with the specific overloading of printf)
 *
 */
struct DataBuffer {
  const char *data;    // Pointer to the data
  const usize &length; // Length of the buffer

  DataBuffer(const char *d, const usize &l) : data(d), length(l) {}
};

/**
 * @brief Consteval class that prints formatted strings
 */
class Format final {

  // Enumeration of the available specifiers
  enum Specifier : char {
    Unknown = 0,

    SignedDecimalInteger = 'd',
    UnsignedDecimalInteger = 'u',
    UnsignedHexadecimalInteger = 'X',
    Character = 'c',
    StringOfCharacters = 's',
    PointerAddress = 'p',
    Time = 't',
    Boolean = 'b'
  };

  // Array of the available specifiers
  static constexpr Specifier specifiers[] = {Specifier::SignedDecimalInteger,
                                             Specifier::UnsignedDecimalInteger,
                                             Specifier::UnsignedHexadecimalInteger,
                                             Specifier::Character,
                                             Specifier::StringOfCharacters,
                                             Specifier::PointerAddress,
                                             Specifier::Time,
                                             Specifier::Boolean};
  /**
   * @brief Inner structure of the specifier (inside provided string) properties
   *
   */
  struct SpecifierData {
    Specifier specifier; // The code of the specifier
    unsigned width;      // For decimal with can be passed (example: %04u)
    unsigned position;   // Position inside string array
    unsigned size;       // The size in elements of the specifier (example: %d - 2, %06u - 4)

    consteval SpecifierData() : specifier(), width(), position(), size() {}
  };

  /**
   * @brief   Inner class that contains SpecifierData for each specifier in the passed string
   *
   * @tparam  N Quantity of specifiers in the string
   */
  template <const unsigned N> struct SpecifierTable {
    SpecifierData data[N];
    static constexpr auto size = N;

    template <typename S>
    requires const_string<S>
    consteval SpecifierTable(const S) {
      constexpr auto IsDigit = [=](const char sym) -> bool { return (('0' <= sym) && (sym <= '9')) ? true : false; };

      unsigned pos = 0;
      for (unsigned i = 0; i < sizeof(S::string); i++) {
        if ('%' == S::string[i]) {
          Specifier specifier = Specifier::Unknown;
          unsigned width = 0;
          unsigned position = i;
          unsigned sz = i;
          while (IsDigit(S::string[++i])) {
            if (('0' == S::string[i]) && (0 == width))
              continue;
            width = (10U * width) + unsigned(S::string[i] - '0');
          }
          char specInString = S::string[i];
          for (const auto &s : specifiers) {
            if (s == specInString) {
              specifier = s;
            }
          }
          data[pos].specifier = specifier;
          data[pos].width = width;
          data[pos].position = position;
          data[pos].size = i - sz + 1;
          pos++;
        }
      }
    }
  };

  /**
   * @brief   Inner function that counts the specifiers quantity in the passed string
   *
   * @tparam  S string
   * @return  Quantity of the specifiers
   */
  template <typename S>
  requires const_string<S>
  static consteval unsigned SpecifierQuantity(const S) {
    unsigned quantity = 0;
    for (const auto &s : S::string) {
      if ('%' == s) {
        quantity++;
      }
    }
    return quantity;
  }

  // Transform width of the specifier to the type property
  template <const unsigned w> struct Width {
    static constexpr unsigned width = w;
  };

  /**
   * @brief   Inner templates that check and format the specifiers according to the passed string
   *
   * @tparam  sp Specifier
   * @tparam  Type Passed type
   */
  template <const Specifier sp, typename Type> struct SpecCheck {
    static constexpr auto valid = false; // Compile time checking for the validity of passed type according to the passed specifier
    static constexpr auto length = 0UL;  // Max expected length
    /**
     * @brief         Formats args for the string in run-time
     *
     * @tparam        W Width as a type property
     * @param buffer  Current buffer position in the result string
     * @param arg     Current argument
     * @return        Length of the result of the format
     */
    template <typename W> static usize formatArg(char *buffer, Type arg, const W);
  };

  // Overload for the signed decimals
  template <typename Type> struct SpecCheck<Specifier::SignedDecimalInteger, Type> {
    static constexpr auto valid = std::is_signed_v<Type> && std::is_integral_v<Type>;
    static constexpr auto length = (3 * sizeof(Type)) - (sizeof(Type) / 2) + 1;
    static_assert(valid, "ERROR: The '%d' specifier supports only signed integrals!");
    template <typename W> static usize formatArg(char *buffer, Type arg, const W) {
      usize num = 0;
      auto uArg = (arg < 0) ? -arg : arg;
      do {
        const unsigned char val = static_cast<unsigned char>(uArg % 10);
        buffer[num++] = val + '0';
      } while (uArg /= 10);

      if constexpr (W::width) {
        while (num < W::width) {
          buffer[num++] = '0';
        }
      }

      if (arg < 0) {
        buffer[num++] = '-';
      }

      char swap;
      for (usize i = 0; i < num / 2; i++) {
        swap = buffer[i];
        buffer[i] = buffer[num - i - 1];
        buffer[num - i - 1] = swap;
      }

      return num;
    }
  };

  // Overload for the unsigned decimals
  template <typename Type> struct SpecCheck<Specifier::UnsignedDecimalInteger, Type> {
    static constexpr auto valid = std::is_unsigned_v<Type> && std::is_integral_v<Type>;
    static constexpr auto length = (3 * sizeof(Type)) - (sizeof(Type) / 2);
    static_assert(valid, "ERROR: The '%u' specifier supports only unsigned integrals!");
    template <typename W> static usize formatArg(char *buffer, Type arg, const W) {
      usize num = 0;
      do {
        const unsigned char val = static_cast<unsigned char>(arg % 10);
        buffer[num++] = val + '0';
      } while (arg /= 10);

      if constexpr (W::width) {
        while (num < W::width) {
          buffer[num++] = '0';
        }
      }

      char swap;
      for (usize i = 0; i < num / 2; i++) {
        swap = buffer[i];
        buffer[i] = buffer[num - i - 1];
        buffer[num - i - 1] = swap;
      }

      return num;
    }
  };

  // Overload for the unsigned hexadecimals
  template <typename Type> struct SpecCheck<Specifier::UnsignedHexadecimalInteger, Type> {
    static constexpr auto valid = std::is_unsigned_v<Type> && std::is_integral_v<Type>;
    static constexpr auto length = 2 * sizeof(Type) + 2;
    static_assert(valid, "ERROR: The '%X' specifier supports only unsigned integrals!");
    template <typename W> static usize formatArg(char *buffer, Type arg, const W) {
      buffer[0] = '0';
      buffer[1] = 'x';
      for (usize i = 2; i < length; i++) {
        const unsigned char val = (arg >> ((8 * sizeof(Type)) - ((i - 1) * 4))) & 0xF;
        buffer[i] = (val < 0xA) ? val + '0' : val + ('A' - 0xA);
      }
      return length;
    }
  };

  // Overload for the unsigned characters
  template <typename Type> struct SpecCheck<Specifier::Character, Type> {
    static constexpr auto valid = std::is_same_v<char, std::remove_cv_t<Type>>;
    static constexpr auto length = sizeof(char);
    static_assert(valid, "ERROR: The '%c' specifier supports only (volatile/const) char)!");
    template <typename W> static usize formatArg(char *buffer, Type arg, const W) {
      buffer[0] = arg;
      return length;
    }
  };

  // Overload for the compile-time strings
  template <typename Type> struct SpecCheck<Specifier::StringOfCharacters, Type> {
    static constexpr auto valid = is_string_v<Type>;
    static constexpr auto length = sizeof(Type::string) - 1;
    static_assert(valid, "ERROR: The '%s' specifier supports only String!");
    template <typename W> static usize formatArg(char *buffer, Type, const W) {
      for (usize i = 0; i < length; i++) {
        buffer[i] = Type::string[i];
      }
      return length;
    }
  };

  // Overload for the pointer addresses
  template <typename Type> struct SpecCheck<Specifier::PointerAddress, Type> {
    static constexpr auto valid = std::is_pointer_v<Type>;
    static constexpr auto length = 2 * sizeof(void *) + 2;
    static_assert(valid, "ERROR: The '%p' specifier supports only pointers!");
    template <typename W> static usize formatArg(char *buffer, Type arg, const W) {
      buffer[0] = '0';
      buffer[1] = 'x';
      const usize uArg = reinterpret_cast<usize>(arg);
      for (usize i = 2; i < length; i++) {
        const unsigned char val = (uArg >> ((8 * sizeof(void *)) - ((i - 1) * 4))) & 0xF;
        buffer[i] = (val < 0xA) ? val + '0' : val + ('A' - 0xA);
      }
      return length;
    }
  };

  // Overload for the time (considered as time from launch)
  template <typename Type> struct SpecCheck<Specifier::Time, Type> {
    static constexpr auto valid = std::is_unsigned_v<Type> && std::is_integral_v<Type> && (sizeof(Type) >= sizeof(usize));
    static constexpr auto length = (3 * sizeof(Type)) - (sizeof(Type) / 2) + 4;
    static_assert(valid, "ERROR: The '%t' specifier supports only unsigned integrals with size >= unsigned long!");
    template <typename W> static usize formatArg(char *buffer, Type arg, const W) {
      auto num = SpecCheck<Specifier::UnsignedDecimalInteger, Type>::formatArg(buffer, arg / 1000, Width<0U>{});
      buffer[num++] = '.';
      num += SpecCheck<Specifier::UnsignedDecimalInteger, Type>::formatArg(&buffer[num], arg % 1000, Width<3U>{});
      return num;
    }
  };

  // Overload for the time booleans
  template <typename Type> struct SpecCheck<Specifier::Boolean, Type> {
    static constexpr auto valid = std::is_convertible_v<Type, bool>;
    static constexpr auto length = sizeof("FALSE");
    static_assert(valid, "ERROR: The '%b' specifier supports only types that can be convertible to bool!");
    template <typename W> static usize formatArg(char *buffer, Type arg, const W) {
      const bool cond = (arg) ? 1 : 0;
      const auto len = (cond ? sizeof("TRUE") : sizeof("FALSE")) - 1;
      static constexpr char const *value[] = {"FALSE", "TRUE"};
      for (usize i = 0; i < len; i++) {
        buffer[i] = value[cond][i];
      }
      return len;
    }
  };

  // Check provided types according to the specifiers in the string, returns max possible length
  template <const auto table, typename First, typename... Rest> static consteval usize CheckSpecsTypes(const First, const Rest...) {
    constexpr auto tableIndex = table.size - (sizeof...(Rest) + 1);
    if constexpr (sizeof...(Rest)) {
      return CheckSpecsTypes<table>(Rest{}...) + SpecCheck<table.data[tableIndex].specifier, First>::length;
    }
    return SpecCheck<table.data[tableIndex].specifier, First>::length;
  }

public:
  /**
   * @brief Consteval constructor for the object
   */
  consteval Format() = default;

  /**
   * @brief         The main fuction of the formatter, passes string to the ostream
   *                Checks and calculations are performed in compile-time (if it is possible)
   *
   * @param str     Compile time string string with the specifiers to be formatted
   * @param args    Variables that should be formatted and placed inside string
   *
   * @example       printf(iso::format::string<"I am a MESSAGE message!">)
   * @example       printf(iso::format::string<"I am a FATAL message! I have one dec %d and one hex %X values!">, -555, 0x1234U);
   * @example       printf(iso::format::string<"I am an ERROR message! I have one unsigned decimal with three 0: %08u">, 98765U);
   * @example       printf(iso::format::string<"I am a WARN message! I have a chars: %c%c%c%c">, 'T', 'E', 'S', 'T');
   *
   * @return        Number of the written symbols
   */
  template <typename S, typename... Args>
  requires const_string<S>
  inline usize printf(const S str, const Args... args) const {
    // General check fot the specifiers quantity the same as the quantity of arguments
    static constexpr auto specifiersQuantity = SpecifierQuantity(str);
    static_assert(sizeof...(args) == specifiersQuantity,
                  "ERROR: The quantity of the specifiers in the string is not the same as the quantity of arguments!");

    // Form specifiers structure
    constexpr SpecifierTable<specifiersQuantity> table(str);
    constexpr bool checkWidth = [&]() {
      for (const auto &d : table.data) {
        if (((d.specifier != Specifier::SignedDecimalInteger) && (d.specifier != Specifier::UnsignedDecimalInteger)) && d.width) {
          return false;
        }
      }
      return true;
    }();
    static_assert(checkWidth, "ERROR: The only decimals allow to have a width!");

    // Check specifiers type and calculate length
    static constexpr auto length = CheckSpecsTypes<table>(Args{}...) + sizeof(S::string);

    // Buffer for the result string
    char buffer[length];

    // Parsing string
    usize counterSource = 0;
    usize counterResult = 0;
    auto parse = [&]<typename First, typename... Rest>(auto &&parse, const First first, const Rest... rest) -> void {
      constexpr auto tableIndex = table.size - (sizeof...(Rest) + 1);
      while (counterSource < table.data[tableIndex].position) {
        buffer[counterResult++] = S::string[counterSource++];
      }

      counterResult +=
          SpecCheck<table.data[tableIndex].specifier, First>::formatArg(&buffer[counterResult], first, Width<table.data[tableIndex].width>{});
      counterSource += table.data[tableIndex].size;

      if constexpr (sizeof...(Rest)) {
        parse(parse, rest...);
      } else {
        while (counterSource < sizeof(S::string)) {
          buffer[counterResult++] = S::string[counterSource++];
        }
      }
    };
    parse(parse, args...);

    auto *stream = Ostream::Get();
    if (stream) [[likely]] {
      (*stream) << buffer;
    }

    // Pass result to the output
    return counterResult;
  }

  /**
   * @brief   Overload for the compile-time string without arguments
   * @return  Number of the written symbols
   */
  template <typename S>
  requires const_string<S>
  inline usize printf(const S) const {
    const auto *stream = Ostream::Get();
    if (stream) [[likely]] {
      stream << S::string;
    }
    return sizeof(S::string);
  }

  /**
   * @brief             Overload for the data buffers
   *
   * @tparam S
   * @param str         Compile time string string with the specifiers to be formatted
   * @param dataBuffer  Object with dynamic data
   * @param args        Variables that should be formatted and placed inside string
   *
   * @example           printf(iso::format::string<"This is just a buffer len [%u]: ">, iso::format::DataBuffer(buf, sizeof(buf)), sizeof(buf));
   *
   * @return            Number of the written symbols
   */
  /*template <typename S, typename... Args>
  requires const_string<S>
  inline usize printf(const S str, const DataBuffer &dataBuffer, const Args... args) const {
    if (nullptr == dataBuffer.data) {
      return 0;
    }
    const auto len = printf(str, args...);
    char buffer[] = " XX";
    char val = '\0';
    for (usize i = 0; i < dataBuffer.length; i++) {
      val = dataBuffer.data[i] >> 4;
      buffer[1] = (val < 0xA) ? val + '0' : val + ('A' - 0xA);
      val = dataBuffer.data[i] & 0xF;
      buffer[2] = (val < 0xA) ? val + '0' : val + ('A' - 0xA);
      puts.puts(buffer, dataBuffer.length);
    }
    puts.puts("\n", sizeof("\n"));
    return (sizeof(buffer) - 1) * dataBuffer.length + len + (sizeof("\n") - 1);
  }*/

  /**
   * @brief   Print line (printf with added "\n" at the end of string in the compile-time)
   * @return  Number of the written symbols
   */
  template <typename S, typename... Args>
  requires const_string<S>
  inline usize println(const S, const Args... args) const {
    constexpr auto res_str = string<S::string> + string<"\n">;
    return printf(res_str, args...);
  }
};
} // namespace isoeng::log