CPMAddPackage(
        NAME fmt
        GITHUB_REPOSITORY fmtlib/fmt
        GIT_TAG 8445327c84dd0bfd23646dc2d92fed105b8fae8c
)

# We need Jinja2 for Glad 2 to work
find_package(Python3 REQUIRED COMPONENTS Interpreter)
execute_process(COMMAND ${Python3_EXECUTABLE} -m pip show Jinja2 RESULT_VARIABLE EXIT_CODE OUTPUT_QUIET)
if (NOT ${EXIT_CODE} EQUAL 0)
    message(FATAL_ERROR "The Jinja2 Python package is not installed. Please install it using the following command: \"${Python3_EXECUTABLE} -m pip install jinja2\".")
endif ()
CPMAddPackage(
        NAME glad
        GITHUB_REPOSITORY Dav1dde/glad
        VERSION 2.0.6
        DOWNLOAD_ONLY YES
)
add_subdirectory(${glad_SOURCE_DIR}/cmake ${glad_BINARY_DIR})
if (WIN32)
    glad_add_library(glad_gl_core_mx_43 REPRODUCIBLE MX API gl:core=4.3 wgl=1.0)
else ()
    glad_add_library(glad_gl_core_mx_43 REPRODUCIBLE MX API gl:core=4.3 glx=1.4 egl=1.5)
endif ()

CPMAddPackage(
        NAME SDL
        GITHUB_REPOSITORY libsdl-org/SDL
        GIT_TAG f01d4278c66e065e64bc934cd01e4f2952f613d7
        OPTIONS "SDL_STATIC ON" "SDL_SHARED OFF"
)

add_library(brot_thirdparty INTERFACE)

target_link_libraries(brot_thirdparty INTERFACE
        fmt::fmt
        glad_gl_core_mx_43
        SDL3::SDL3
)
