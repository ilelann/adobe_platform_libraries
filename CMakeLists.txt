cmake_minimum_required(VERSION 3.0)
include(FindPkgConfig)
enable_testing()

project(adobe_platform_libraries CXX)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++0x")
set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")

add_definitions ("-Wall")
add_definitions ("-Werror")

add_subdirectory(../adobe_source_libraries ${CMAKE_BINARY_DIR}/imported/asl)

aux_source_directory(backend/gtk/adobe/future/widgets/sources PLATFORM_FUTURE_WIDGET_SOURCES)
aux_source_directory(backend/gtk/adobe/future/source PLATFORM_FUTURE_SOURCES)
aux_source_directory(backend/template/ PLATFORM_FUTURE_TEMPLATES)
aux_source_directory(adobe/future/widgets/sources FUTURE_WIDGET_SOURCES)
aux_source_directory(adobe/future/source FUTURE_SOURCES)
aux_source_directory(source NON_FUTURE_SOURCES)


file(GLOB_RECURSE PLATFORM_INC_LIST backend/*.hpp)
file(GLOB_RECURSE INC_LIST adobe/*.hpp)

# not yet handled in current GTK backend
list(REMOVE_ITEM FUTURE_WIDGET_SOURCES
        "adobe/future/widgets/sources/platform_presets.cpp"
        "adobe/future/widgets/sources/presets_factory.cpp"
        "adobe/future/widgets/sources/presets_common.cpp"
        "adobe/future/widgets/sources/link_factory.cpp"
        "adobe/future/widgets/sources/image_factory.cpp"
        )

# not needed in current state
list(REMOVE_ITEM FUTURE_SOURCES
        "adobe/future/source/cmd_system.cpp"
        "adobe/future/source/drag_and_drop.cpp"
        "adobe/future/source/menu_system.cpp"
        )

# not needed in current state
list(REMOVE_ITEM NON_FUTURE_SOURCES
        "source/expression_formatter.cpp"
        "source/formatter_tokens.cpp"
        "source/layout_formatter.cpp"
        "source/property_model_formatter.cpp"
        )

add_library(apl 
    ${PLATFORM_FUTURE_WIDGET_SOURCES}
    ${PLATFORM_FUTURE_SOURCES}
    ${PLATFORM_FUTURE_TEMPLATES}
    ${FUTURE_WIDGET_SOURCES}
    ${FUTURE_SOURCES}
    ${NON_FUTURE_SOURCES}
    ${PLATFORM_INC_LIST}
    ${INC_LIST})

target_compile_definitions(apl PUBLIC ADOBE_PLATFORM_GTK)
# name based (vs image based) toggle widget in current GTK backend
target_compile_definitions(apl PRIVATE ADOBE_PLATFORM_TOGGLE_NAME)

# not yet handled in current GTK backend (see source files excluded above)
target_compile_definitions(apl PRIVATE ADOBE_PLATFORM_NO_PRESETS)
target_compile_definitions(apl PRIVATE ADOBE_PLATFORM_NO_LINK)
target_compile_definitions(apl PRIVATE ADOBE_PLATFORM_NO_IMAGE)

# for view_concept and controller_concept checks
target_compile_options(apl PUBLIC "-Wno-unused-local-typedefs")


# windows backend for convenience
aux_source_directory(windows/adobe/future/widgets/sources WIN_PLATFORM_FUTURE_WIDGET_SOURCES)
aux_source_directory(windows/adobe/future/source WIN_PLATFORM_FUTURE_SOURCES)
file(GLOB_RECURSE WIN_PLATFORM_INC_LIST windows/*.hpp)
add_library(windows EXCLUDE_FROM_ALL
    ${WIN_PLATFORM_FUTURE_WIDGET_SOURCES}
    ${WIN_PLATFORM_FUTURE_SOURCES}
    ${WIN_PLATFORM_INC_LIST})


# macintosh backend for convenience
aux_source_directory(macintosh/adobe/future/widgets/sources MAC_PLATFORM_FUTURE_WIDGET_SOURCES)
aux_source_directory(macintosh/adobe/future/source MAC_PLATFORM_FUTURE_SOURCES)
file(GLOB_RECURSE MAC_PLATFORM_INC_LIST macintosh/*.hpp)
add_library(macintosh EXCLUDE_FROM_ALL
    ${MAC_PLATFORM_FUTURE_WIDGET_SOURCES}
    ${MAC_PLATFORM_FUTURE_SOURCES}
    ${MAC_PLATFORM_INC_LIST})


target_include_directories(apl PUBLIC .)
target_include_directories(apl PUBLIC backend/template)
target_include_directories(apl PUBLIC backend/gtk)

target_link_libraries(apl PUBLIC asl)

pkg_check_modules(GTK gtk+-3.0)
target_compile_options(apl PUBLIC ${GTK_CFLAGS})
target_link_libraries(apl PUBLIC ${GTK_LIBRARIES})

add_subdirectory(test)
