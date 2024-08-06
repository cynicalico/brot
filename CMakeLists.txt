cmake_minimum_required(VERSION 3.30 FATAL_ERROR)
project(brot)

add_library(brot STATIC)

target_sources(brot PUBLIC
        FILE_SET CXX_MODULES FILES
        src/brot/engine.cppm
        src/brot/window.cppm
)

target_compile_features(brot PUBLIC cxx_std_23)

include(cmake/get_cpm.cmake)
include(cmake/get_thirdparty.cmake)
target_link_libraries(brot PUBLIC brot_thirdparty)

add_subdirectory(example)
