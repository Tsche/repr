#pragma once
#include <librepr/feature.h>

#ifndef REPR_FORMAT_FMT
#define REPR_FORMAT_FMT OFF
#endif

#if USING(REPR_FORMAT_FMT)
#include <fmt/format.h>
#define REPR_FORMAT_NS ::fmt
#else
#include <format>
#define REPR_FORMAT_NS ::std
#endif

#define REPR_FORMAT(...) REPR_FORMAT_NS::format(__VA_ARGS__)