target_sources(repr PRIVATE 
  fundamental.cpp
  pointer.cpp  
)

IF (WIN32)
  target_sources(repr PRIVATE demangle.windows.cpp)
ELSE()
  target_sources(repr PRIVATE demangle.linux.cpp)
ENDIF()