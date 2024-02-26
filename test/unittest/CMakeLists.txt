target_sources(repr_test PRIVATE
        main.cpp
)

add_subdirectory(util)
add_subdirectory(repr)
add_subdirectory(name)
add_subdirectory(layout)
add_subdirectory(reflection)
add_subdirectory(visitors)
add_subdirectory(enum)
add_subdirectory(ctvi)

include(warnings.cmake)
enable_warnings(repr_test)