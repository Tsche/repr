target_sources(repr_test PRIVATE 
  main.cpp
)

add_subdirectory(detail)
add_subdirectory(repr)
add_subdirectory(name)
add_subdirectory(layout)
add_subdirectory(reflection)
add_subdirectory(visitors)