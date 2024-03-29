#include <algorithm>
#include <string_view>
#include <concepts>

namespace librepr::util {

namespace detail {
template <typename SubType>
struct StrExpr {
    constexpr SubType const* operator->() const {
        return static_cast<SubType const*>(this);
    }

    constexpr SubType const& operator*() const {
        return *static_cast<SubType const*>(this);
    }
};

struct LiteralTag {};
struct ArrayTag {};


template <typename T>
concept copyable_string = requires (T str){
    {str.size} -> std::convertible_to<std::size_t>;
    {str.copy_to(std::declval<char*>(), 0U)};
};
}

template <std::size_t N, typename Tag>
struct meta_string;

template <std::size_t N>
struct meta_string<N, detail::LiteralTag> : public detail::StrExpr<meta_string<N, detail::LiteralTag>> {
    const char (&value)[N + 1];
    constexpr static auto size = N;

    constexpr meta_string(char const (&literal)[N + 1]) : value(literal) {}

    constexpr std::size_t copy_to(char* destination, std::size_t offset) const {
        std::copy_n(value, N, destination + offset);
        return N;
    }

    constexpr auto operator*() const { return value; }
};

template <std::size_t N>
struct meta_string<N, detail::ArrayTag> : public detail::StrExpr<meta_string<N, detail::ArrayTag>> {
    char value[N + 1]{};
    constexpr static auto size = N;

    constexpr meta_string() = default;

    constexpr explicit meta_string(std::string_view data) {
        std::copy_n(data.data(), N, std::begin(value));
    }

    constexpr meta_string(std::initializer_list<char> data) {
        std::copy(data.begin(), data.end(), value);
    }

    constexpr std::size_t copy_to(char* destination, std::size_t offset) const {
        std::copy_n(value, N, destination + offset);
        return N;
    }

    constexpr auto operator*() const { return value; }
};

template <std::size_t N>
meta_string(char const (&)[N]) -> meta_string<N - 1, detail::LiteralTag>;

template <std::same_as<char>... Ts>
meta_string(Ts...) -> meta_string<sizeof...(Ts), detail::ArrayTag>;

template <typename Lhs, typename Rhs>
class StrAppendExpr : public detail::StrExpr<StrAppendExpr<Lhs, Rhs>> {
    Lhs lhs;
    Rhs rhs;

   public:
    constexpr static auto size = Lhs::size + Rhs::size;

    constexpr StrAppendExpr(const Lhs& lhs, const Rhs& rhs)
        : lhs(lhs), rhs(rhs) {}

    constexpr std::size_t copy_to(char* destination, std::size_t offset) const {
        lhs.copy_to(destination, offset);
        rhs.copy_to(destination, offset + lhs.size);
        return rhs.size + lhs.size;
    }
};

template <typename Self, typename Other>
constexpr auto operator+(detail::StrExpr<Self> const& lhs, detail::StrExpr<Other> const& rhs) {
    if constexpr (Self::size == 0) {
        return *rhs;
    } else if constexpr (Other::size == 0) {
        return *lhs;
    } else {
        return StrAppendExpr<Self, Other>{*lhs, *rhs};
    }
}

template <std::size_t N>
using string_literal = meta_string<N, detail::LiteralTag>;

template <std::size_t N>
using interned_string = meta_string<N, detail::ArrayTag>;

template <typename Self>
constexpr auto intern(detail::StrExpr<Self> const& str) {
    auto string = interned_string<Self::size>();
    str->copy_to(string.value, 0U);
    return string;
}

constexpr auto concat(detail::copyable_string auto const&... parts){
  auto buffer = interned_string<(parts.size + ...)>();
  auto pos = 0U;
  ((pos += parts.copy_to(buffer.value, pos)), ...);
  return buffer;
}

}