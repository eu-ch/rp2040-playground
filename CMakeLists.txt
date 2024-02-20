cmake_minimum_required(VERSION 3.24)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --------------------------------------------------
# Set up Pico SDK

include(tools/build_tool/cmake/pico_sdk_import.cmake)
include(tools/build_tool/cmake/pico_extras_import_optional.cmake)
include(tools/build_tool/cmake/example_auto_set_url.cmake)

project(rp2040-playground C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

pico_sdk_init()

add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        )
if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wno-maybe-uninitialized)
endif()

# --------------------------------------------------
# Extras

add_subdirectory(third_party/pico-extras)

# --------------------------------------------------
# Programs

add_subdirectory(programs/heavy_test)
add_subdirectory(programs/heavyflop)
add_subdirectory(programs/i2s_test)
