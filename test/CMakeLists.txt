cmake_minimum_required(VERSION 2.6)

project(btsnoop-test)

if(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32 -Wall -std=c++11")
endif()

set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})

include_directories(
		../include
		include
)

link_directories(../lib/${CMAKE_BUILD_TYPE})


file(
        GLOB_RECURSE
        source_files
        src/*
        include/*
)

add_executable(
        btsnoop-test
        ${source_files}
)

target_link_libraries(
        btsnoop-test
        btsnoop
        pthread
)