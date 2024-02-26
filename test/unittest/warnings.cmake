# Build flags
# See:
# - https://github.com/aminya/project_options/blob/main/src/CompilerWarnings.cmake
# - https://github.com/cpp-best-practices/cppbestpractices/blob/master/02-Use_the_Tools_Available.md
# - https://clang.llvm.org/docs/DiagnosticsReference.html
# - https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
# - https://learn.microsoft.com/en-us/cpp/build/reference/compiler-options-listed-by-category?view=msvc-170
# - https://developers.redhat.com/blog/2018/03/21/compiler-and-linker-flags-gcc

function(enable_warnings _TARGET)
  if("${MSVC_WARNINGS}" STREQUAL "")
    set(MSVC_WARNINGS
      /W4 # Baseline reasonable warnings
      /w14242 # 'identifier': conversion from 'type1' to 'type1', possible loss of data
      /w14254 # 'operator': conversion from 'type1:field_bits' to 'type2:field_bits', possible loss of data
      /w14263 # 'function': member function does not override any base class virtual member function
      /w14265 # 'classname': class has virtual functions, but destructor is not virtual instances of this class may not

      # be destructed correctly
      /w14287 # 'operator': unsigned/negative constant mismatch
      /we4289 # nonstandard extension used: 'variable': loop control variable declared in the for-loop is used outside

      # the for-loop scope
      /w14296 # 'operator': expression is always 'boolean_value'
      /w14311 # 'variable': pointer truncation from 'type1' to 'type2'
      /w14545 # expression before comma evaluates to a function which is missing an argument list
      /w14546 # function call before comma missing argument list
      /w14547 # 'operator': operator before comma has no effect; expected operator with side-effect
      /w14549 # 'operator': operator before comma has no effect; did you intend 'operator'?
      /w14555 # expression has no effect; expected expression with side- effect
      /w14619 # pragma warning: there is no warning number 'number'
      /w14640 # Enable warning on thread un-safe static member initialization
      /w14826 # Conversion from 'type1' to 'type_2' is sign-extended. This may cause unexpected runtime behavior.
      /w14905 # wide string literal cast to 'LPSTR'
      /w14906 # string literal cast to 'LPWSTR'
      /w14928 # illegal copy-initialization; more than one user-defined conversion has been implicitly applied
      /permissive- # standards conformance mode for MSVC compiler.
    )
  endif()

  set(COMMON_WARNINGS
    -Wall
    -Wextra # reasonable and standard
    -Wextra-semi # Warn about semicolon after in-class function definition.
    -Wshadow # warn the user if a variable declaration shadows one from a parent context
    -Wnon-virtual-dtor # warn the user if a class with virtual functions has a non-virtual destructor. This helps

    # catch hard to track down memory errors
    -Wold-style-cast # warn for c-style casts
    -Wcast-align # warn for potential performance problem casts
    -Wunused # warn on anything being unused
    -Woverloaded-virtual # warn if you overload (not override) a virtual function
    -Wpedantic # warn if non-standard C++ is used
    -Wconversion # warn on type conversions that may lose data
    -Wsign-conversion # warn on sign conversions
    -Wnull-dereference # warn if a null dereference is detected
    -Wdouble-promotion # warn if float is implicit promoted to double
    -Wformat=2 # warn on security issues around functions that format output (ie printf)
    -Wimplicit-fallthrough # warn on statements that fallthrough without an explicit annotation
  )

  if("${CLANG_WARNINGS}" STREQUAL "")
    set(CLANG_WARNINGS
      ${COMMON_WARNINGS}
      -Wno-dollar-in-identifier-extension
      -Wno-unknown-attributes # ignore unknown attribute warning
    )
  endif()

  if("${GCC_WARNINGS}" STREQUAL "")
    set(GCC_WARNINGS
      ${COMMON_WARNINGS}
      -Wmisleading-indentation # warn if indentation implies blocks where blocks do not exist
      -Wduplicated-cond # warn if if / else chain has duplicated conditions
      -Wduplicated-branches # warn if if / else branches have duplicated code
      -Wlogical-op # warn about logical operations being used where bitwise were probably wanted
      -Wuseless-cast # warn if you perform a cast to the same type
      -Wno-attributes # ignore unknown attribute warning
      # -fconcepts-diagnostics-depth=2
    )
  endif()

  if(MSVC)
    message(STATUS "Setting warning flags for MSVC")
    set(PROJECT_WARNINGS ${MSVC_WARNINGS})

  elseif(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    message(STATUS "Setting warning flags for clang")
    set(PROJECT_WARNINGS ${CLANG_WARNINGS})

  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(STATUS "Setting warning flags for GCC")
    set(PROJECT_WARNINGS ${GCC_WARNINGS})

  else()
    message(AUTHOR_WARNING "No compiler warnings set for CXX compiler: '${CMAKE_CXX_COMPILER_ID}'")
  endif()

  target_compile_options(${_TARGET} PUBLIC ${PROJECT_WARNINGS})
endfunction()
