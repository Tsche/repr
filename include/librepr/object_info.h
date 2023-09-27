#pragma once
#include <string>
#include <format>
#include <concepts>
#include <sstream>
#include <vector>

#include <librepr/type_info.h>

namespace librepr {

struct Literal {
  std::string value{};

  [[nodiscard]] std::string to_string(bool /* with_type */) const { return value; }
};

struct OptionalSuffix {
  std::string value{};
  std::string suffix{};

  [[nodiscard]] std::string to_string(bool with_type = false) const {
    if (with_type) {
      return std::format("{{{}}}", value);
    }
    return value + suffix;
  }
};

template <typename... Args>
  requires(requires(Args arg) {
    { arg.type.name() } -> std::same_as<std::string>;
    { arg.to_string() } -> std::same_as<std::string>;
  } && ...)
struct BracedList {
  std::tuple<Args...> value;

  explicit BracedList(Args const&... args) : value(std::tie(args...)) {}

  [[nodiscard]] std::string to_string(bool with_type = false) const {
    std::ostringstream stream;
    stream << '{';
    std::apply(
        [&stream, with_type](auto&&... args) {
          [[maybe_unused]] const char* delimiter = "";
          (((stream << delimiter << args.to_string(with_type, with_type)), delimiter = ", "), ...);
        },
        value);

    stream << '}';
    return stream.str();
  }
};

template <typename Inner>
struct InitializerList {
  std::vector<Inner> values;

  [[nodiscard]] std::string to_string(bool with_type = false) const {
    std::ostringstream list{};

    list << '{';
    for (auto const& element : values) {
      if (&element != &*std::begin(values)) {
        list << ", ";
      }

      list << element.to_string(with_type, with_type);
    }
    list << '}';
    return list.str();
  }
};

template <typename Value, typename Repr>
struct ObjectInfo {
  using ValueType = Value;

  Value const& value;
  Repr repr;
  static constexpr TypeInfo<Value> type{};

  [[nodiscard]] std::string to_string(bool with_type = true, bool explicit_types = false) const {
    if (with_type && not std::same_as<Literal, Repr>) {
      return type.name() + repr.to_string(explicit_types);
    }
    return repr.to_string(explicit_types);
  }

  [[nodiscard]] operator std::string() const { return to_string(); }

  [[nodiscard]] std::string operator()(bool with_type = true, bool explicit_types = false) const {
    return to_string(with_type, explicit_types);
  }

  friend std::ostream& operator<<(std::ostream& ctx, ObjectInfo<Value, Repr> const& obj) {
    return ctx << obj.to_string();
  }

  friend bool operator==(ObjectInfo<Value, Repr> const& obj, std::string_view other){
    return obj.to_string() == other;
  }
};

template <typename T, typename V>
ObjectInfo(T const&, V) -> ObjectInfo<T, V>;

template <typename T>
ObjectInfo(T const&, char const*) -> ObjectInfo<T, Literal>;

template <typename T>
ObjectInfo(T const&, std::string) -> ObjectInfo<T, Literal>;
}  // namespace librepr

// std::format support for librepr::ObjectInfo
template <class T, class Repr, class CharT>
struct std::formatter<librepr::ObjectInfo<T, Repr>, CharT> : std::formatter<std::string, CharT> {  // NOLINT
  auto format(librepr::ObjectInfo<T, Repr> const& obj, auto& ctx) const {
    return std::formatter<std::string, CharT>::format(obj.to_string(), ctx);
  }
};
