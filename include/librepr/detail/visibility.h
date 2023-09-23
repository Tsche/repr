#pragma once
#include "default.h"

#if defined _WIN32 || defined __CYGWIN__

  #ifdef EXPORT_SYMBOLS
    #define EXPORT __declspec(dllexport)
  #else
      #define EXPORT __declspec(dllimport)
  #endif

#else

  #define EXPORT __attribute__ ((visibility ("default")))

#endif
