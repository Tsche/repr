#pragma once
#include <array>
#include <cstdlib>
#include <memory>
#include <librepr/detail/macros.h>

// Reference: https://github.com/nihilus/IDA_ClassInformer

#ifndef REPR_DEMANGLE_MAX
# define REPR_DEMANGLE_MAX 1024UL
#endif

namespace librepr::detail {
/// Wrapped buffer for demangling
using DemangleBuffer = std::array<char, REPR_DEMANGLE_MAX>;

namespace msvc {
extern "C" {
  using UndAlloc = void*(__cdecl*)(unsigned int);
  using UndFree  = void(__cdecl*)(void*);
}

  /// Flags for the method of undecoration.
  struct UndStrategy {
   enum : unsigned int {
    Complete              = 0x00000,  // Enable full undecoration
    NoLeadingUnderscores  = 0x00001,  // Remove leading underscores from MS extended keywords
    NoMsKeywords          = 0x00002,  // Disable expansion of MS extended keywords
    NoFunctionReturns     = 0x00004,  // Disable expansion of return type for primary declaration
    NoAllocationModel     = 0x00008,  // Disable expansion of the declaration model
    NoAllocationLanguage  = 0x00010,  // Disable expansion of the declaration language specifier
    NoMSThisType          = 0x00020,  // Disable expansion of MS keywords on the 'this' type for primary declaration
    NoCVThisType          = 0x00040,  // Disable expansion of CV modifiers on the 'this' type for primary declaration
    NoThisType            = 0x00060,  // Disable all modifiers on the 'this' type
    NoAccessSpecifiers    = 0x00080,  // Disable expansion of access specifiers for members
    NoThrowSignatures     = 0x00100,  // Disable expansion of 'throw-signatures' for functions and pointers to functions
    NoMemberType          = 0x00200,  // Disable expansion of 'static' or 'virtual'ness of members
    NoReturnUDTModel      = 0x00400,  // Disable expansion of MS model for UDT returns
    Decode32Bit           = 0x00800,  // Undecorate 32-bit decorated names
    NameOnly              = 0x01000,  // Crack only the name for primary declaration; return just [scope::]name.  Does expand template params
    TypeOnly              = 0x02000,  // Input is just a type encoding; compose an abstract declarator
    HaveParameters        = 0x04000,  // The real templates parameters are available
    NoECSU                = 0x08000,  // Suppress enum/class/struct/union
    NoIdentCharCheck      = 0x10000,  // Suppress check for IsValidIdentChar
   };

    using Type = decltype(Complete);
    using type = decltype(Complete);
  };

  struct UndFreeFunctor {
    void operator()(char* str) const
     noexcept { std::free(str); }
  };

  using UndHandle = std::unique_ptr<char[], UndFreeFunctor>;

  inline void* __cdecl malloc_bridge(unsigned int size) {
    return std::malloc(std::size_t(size));
  }

  extern "C" char* __cdecl __unDName( // NOLINT
    char* buffer, const char* name, int buffer_size,
    UndAlloc allocator, UndFree _free, 
    UndStrategy::Type flags
  );

  /// Bridge for invoking `__unDName`.
  LIBREPR_FORCE_INLINE char* _unDName(const char* name,
   char* buffer, std::size_t buffer_size, UndStrategy::Type flags) {
    LIBREPR_ASSERT(buffer_size <= INT_MAX, 
      "The buffer_size {} is greater than INT_MAX ({}).", buffer_size, INT_MAX);
    return __unDName(buffer, name, int(buffer_size), &malloc_bridge, &std::free, flags);
  }

  /// Returns a managed handle to the demangled buffer.
  inline UndHandle unDName(const char* name, UndStrategy::Type flags) {
    return UndHandle{ _unDName(name, nullptr, 0, flags) };
  }

  /// Writes demangled name directly to `buffer`.
  inline void unDName(const char* name, DemangleBuffer& buffer, UndStrategy::Type flags) {
    [[maybe_unused]] char* ret = _unDName(name, buffer.data(), buffer.size(), flags);
    LIBREPR_ASSERT((buffer.data() == ret), "__unDName output address did not match input buffer address.");
  }
} // namespace msvc

using msvc::UndStrategy;
using msvc::UndHandle;

} // namespace librepr::detail
