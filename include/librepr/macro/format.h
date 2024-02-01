#pragma once
#include <librepr/feature.h>

#ifndef REPR_FORMAT_FMT
#define REPR_FORMAT_FMT OFF
#endif

#if USING(REPR_FORMAT_FMT)
#include <fmt/format.h>
#define REPR_FORMAT_RNS fmt
#else
#include <format>
#define REPR_FORMAT_RNS std
#endif

#define REPR_FORMAT_NS ::REPR_FORMAT_RNS
#define REPR_FORMAT(...) ::REPR_FORMAT_RNS::format(__VA_ARGS__)