cmake_minimum_required (VERSION 2.6)

project(btsnoop-decoder)

if(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32 -Wall -std=c++11")
endif()

set(LIBRARY_OUTPUT_PATH lib/${CMAKE_BUILD_TYPE})

include_directories(include)

file(
        GLOB_RECURSE
        source_files
        src/*
        include/*
)

add_library(
        btsnoop_static
        STATIC
        ${source_files}
)

add_library(
        btsnoop
        SHARED
        ${source_files}
)