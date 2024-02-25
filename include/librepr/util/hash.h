#pragma once
#include <cstdint>
#include <string_view>
#include <utility>
#include <librepr/macro/util.h>
#include <librepr/util/string/util.h>

namespace librepr {

template <typename T>
constexpr inline T fnv_prime;

template <>
constexpr inline std::uint32_t fnv_prime<std::uint32_t> = 16777619UL;

template <>
constexpr inline std::uint64_t fnv_prime<std::uint64_t> = 1099511628211ULL;

template <typename T>
constexpr inline T fnv_offset;

template <>
constexpr inline std::uint32_t fnv_offset<std::uint32_t> = 2166136261UL;

template <>
constexpr inline std::uint64_t fnv_offset<std::uint64_t> = 14695981039346656037ULL;

template <std::size_t... Idx>
constexpr std::uint32_t fnv1a(std::string_view str, std::index_sequence<Idx...>) {
  std::uint32_t hash = fnv_offset<std::uint32_t>;
  (((hash ^= static_cast<unsigned int>(str[Idx])) *= fnv_prime<std::uint32_t>), ...);
  return hash;
}

constexpr std::uint32_t fnv1a(char const* str, std::size_t size) {
  std::uint32_t hash = fnv_offset<std::uint32_t>;
  for (std::size_t idx = 0; idx < size; ++idx) {
    hash ^= static_cast<unsigned char>(str[idx]);
    hash *= fnv_prime<std::uint32_t>;
  }
  return hash;
}

constexpr std::uint32_t fnv1a(std::string_view str) {
  return fnv1a(str.begin(), str.size());
}

template <std::size_t N>
constexpr std::uint32_t fnv1a(char const (&str)[N]) {
  return fnv1a(str, N - 1);
}

consteval std::uint32_t operator""_fnv1a(const char* str, std::size_t size) {
  return fnv1a(str, size);
}

namespace util {
struct Hash {
  // Everything being implicit here is intended. This type is just a conversion helper.
  // NOLINTBEGIN
  constexpr explicit(false) Hash(std::string_view str) : value{fnv1a(str)} {}
  template <std::size_t N>
  constexpr explicit(false) Hash(char const (&str)[N]) : value{fnv1a(str, N - 1)} {}
  constexpr explicit(false) Hash(char const* str) : value{fnv1a(std::string_view{str})} {}
  constexpr explicit(false) Hash(std::uint32_t hash) : value{hash} {}
  constexpr explicit(false) operator std::uint32_t() const { return value; }
  // NOLINTEND

  std::uint32_t value;
};
}  // namespace util

template <std::size_t N>
struct HashSet {
  std::uint32_t set[N];

  HashSet() = delete;

  template <std::convertible_to<util::Hash>... Vs>
    requires(sizeof...(Vs) == N)
  constexpr explicit HashSet(Vs... values) : set{util::Hash{values}...} {}

  constexpr explicit HashSet(std::uint32_t (&set_)[N]) : set{} { std::copy(set_, set_ + N, set); }

  [[nodiscard]] constexpr bool contains(util::Hash hash) const {
    for (auto&& key : set) {
      if (hash.value == key) {
        return true;
      }
    }
    return false;
  }

  [[nodiscard]] std::size_t size() const { return N; }
  [[nodiscard]] std::uint32_t const* begin() const { return set; }
  [[nodiscard]] std::uint32_t const* end() const { return set + N; }
};

template <typename... Vs>
HashSet(Vs...) -> HashSet<sizeof...(Vs)>;

}  // namespace librepr