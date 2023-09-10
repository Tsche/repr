#pragma once
#include <string>
#include "visibility.h"

#include <ranges>
//#include <sstream>

#include <vector>
#include <map>
#include <unordered_map>
#include <utility> 

namespace detail {
template <typename First, typename Second>
std::string to_init_list(std::pair<First, Second> const& obj);

template <std::ranges::range T>
std::string to_init_list(T const& iterable);
}

template <typename T, std::size_t N>
EXPORT std::string repr(T (&&obj)[N]) {
    return std::format("({}(&&)[{}]){{{}}}", 
                       get_name<T>(), 
                       N, 
                       detail::to_init_list(obj));
}

template <typename T, std::size_t N>
EXPORT std::string repr(T (&obj)[N]) {
    return std::format("{{{}}}", detail::to_init_list(obj));
}

template <typename T>
EXPORT std::string repr(std::vector<T> const& obj) {
    return std::format("std::vector<{}>{{{}}}", 
                       get_name<T>(), 
                       detail::to_init_list(obj));
}

template <typename K, typename V>
EXPORT std::string repr(std::map<K, V> const& obj) {
    return std::format("std::map<{}, {}>{{{}}}", 
                       get_name<K>(), 
                       get_name<V>(), 
                       detail::to_init_list(obj));
}

template <typename K, typename V>
EXPORT std::string repr(std::unordered_map<K, V> const& obj) {
    return std::format("std::unordered_map<{}, {}>{{{}}}", 
                       get_name<K>(), 
                       get_name<V>(), 
                       detail::to_init_list(obj));
}

template <typename First, typename Second>
EXPORT std::string repr(std::pair<First, Second> const& obj) {
    return std::format("std::pair<{}, {}>{{{}, {}}}",
                       get_name<First>(), 
                       get_name<Second>(), 
                       repr(obj.first),
                       repr(obj.second));
}

namespace detail {

template <typename First, typename Second>
std::string to_init_list(std::pair<First, Second> const& obj) {
    return std::format("{{{}, {}}}", obj.first, obj.second);
}

template <std::ranges::range T>
std::string to_init_list(T const& obj) {
    using namespace std::string_literals;

    /*return std::views::transform([](auto obj){
                    return std::format("{}", obj);
                  })
                | std::views::join_with()
                | std::ranges::to<std::string>();*/

    std::string list;
    std::ranges::copy(obj | std::views::transform([](auto obj) {
                          if constexpr( requires {to_init_list(obj);}){
                            return to_init_list(obj);
                          }
                          else {
                            return repr(obj);
                          }
                      }) | std::views::join_with(", "s),
                      std::back_inserter(list));
    return list;

    /*
    std::ostringstream list;
    list << '{';
    for (auto const& element : iterable) {
        if (&element == &*std::begin(iterable)) {
            list << repr(element);
            continue;
        }
        list << ", " << repr(element);
    }
    list << '}';
    return list.str();*/
}
}