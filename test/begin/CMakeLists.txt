aux_source_directory(sources SRC_LIST)
add_executable (begin ${SRC_LIST} sources/gtk/main.cpp)
target_include_directories(begin PUBLIC headers)
target_link_libraries(begin PUBLIC apl)

# other backends for convenience
add_library(macintosh_begin EXCLUDE_FROM_ALL sources/mac/main.cpp)
add_library(windows_begin EXCLUDE_FROM_ALL sources/win32/main.cpp)
