cmake_minimum_required (VERSION 2.6)

project(btsnoop-decoder)

set(LIBRARY_OUTPUT_PATH lib/${CMAKE_BUILD_TYPE})

include_directories(headers)

file(
        GLOB_RECURSE
        source_files
        cpp/*
        headers/*
)

add_library(
        btsnoop
        STATIC
        ${source_files}
)
