#pragma once

template <auto V> 
void eval();

template <typename V> 
void eval();

#define $eval(...) static_assert(&eval<__VA_ARGS__>)