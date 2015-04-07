aux_source_directory(sources SRC_LIST)
add_executable (begin ${SRC_LIST} sources/gtk/main.cpp)
target_include_directories(begin PUBLIC headers)
target_link_libraries(begin PUBLIC apl)

