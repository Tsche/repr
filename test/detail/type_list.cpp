#include <concepts>
#include <type_traits>

#include <librepr/util/type_list.h>

namespace librepr::detail {

static_assert(std::same_as<TypeList<>::map<std::type_identity>, TypeList<>>);
static_assert(std::same_as<TypeList<int, char>::map<std::type_identity>,
                           TypeList<std::type_identity<int>, std::type_identity<char>>>);

static_assert(std::same_as<TypeList<>::map_t<std::type_identity>, TypeList<>>);
static_assert(std::same_as<TypeList<int, char>::map_t<std::type_identity>, TypeList<int, char>>);

static_assert(std::same_as<TypeList<int, char, float>::get<0>, int>);
static_assert(std::same_as<TypeList<int, char, float>::get<1>, char>);
static_assert(std::same_as<TypeList<int, char, float>::get<2>, float>);

static_assert(std::same_as<TypeList<int, char, float>::slice<0, 1>, TypeList<int>>);
static_assert(std::same_as<TypeList<int, char, float>::slice<1>, TypeList<char, float>>);
static_assert(std::same_as<TypeList<int, char, float>::slice<1, 2>, TypeList<char>>);
static_assert(std::same_as<TypeList<int, char, float>::slice<1, 1>, TypeList<>>);

static_assert(std::same_as<TypeList<int, char, float>::split<3>, TypeList<TypeList<int, char, float>, TypeList<>>>);
static_assert(std::same_as<TypeList<int, char, float>::split<1>, TypeList<TypeList<int>, TypeList<char, float>>>);
static_assert(std::same_as<TypeList<int, char, float>::split<2>, TypeList<TypeList<int, char>, TypeList<float>>>);
static_assert(std::same_as<TypeList<int, char, float>::split<0>, TypeList<TypeList<>, TypeList<int, char, float>>>);

static_assert(std::same_as<TypeList<>::append<float>, TypeList<float>>);
static_assert(std::same_as<TypeList<>::prepend<float>, TypeList<float>>);
static_assert(std::same_as<TypeList<int, char>::append<float>, TypeList<int, char, float>>);
static_assert(std::same_as<TypeList<int, char>::prepend<float>, TypeList<float, int, char>>);

static_assert(std::same_as<TypeList<>::from_tuple<std::tuple<>>, TypeList<>>);
static_assert(std::same_as<TypeList<>::from_tuple<std::tuple<int, char, float>>, TypeList<int, char, float>>);
static_assert(std::same_as<TypeList<>::to<std::tuple>, std::tuple<>>);
static_assert(std::same_as<TypeList<int, char, float>::to<std::tuple>, std::tuple<int, char, float>>);

static_assert(std::same_as<TypeList<int, char>::map<std::type_identity>,
                           TypeList<std::type_identity<int>, std::type_identity<char>>>);

static_assert(std::same_as<TypeList<int, char>::map_t<std::type_identity>, TypeList<int, char>>);

}  // namespace librepr