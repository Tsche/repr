#pragma once
#include <librepr/feature.h>
#include "util.h"

#ifndef REPR_FORMAT_FMT
#define REPR_FORMAT_FMT ON
#endif

#if !__has_include(<format>)
#undef REPR_FORMAT_FMT
#define REPR_FORMAT_FMT ON
#endif

#if USING(REPR_FORMAT_FMT)

#if !__has_include(<fmt/format.h>)
#error "Configured to use fmt, but fmt was not found."
#endif

#include <fmt/format.h>
#define REPR_FORMAT_RNS fmt
#else
#include <format>
#define REPR_FORMAT_RNS std
#endif

#define REPR_FORMAT_NS ::REPR_FORMAT_RNS
#define REPR_FORMAT(...) ::REPR_FORMAT_RNS::format(__VA_ARGS__)