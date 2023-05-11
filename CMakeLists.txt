cmake_minimum_required(VERSION 3.10)
project(protobuf_zmq)

option(BUILD_DEBUG "Build in Debug Mode" ON)
option(BUILD_RELEASE "BUild in Release Mode" OFF)

if (BUILD_DEBUG)
    set(CMAKE_BUILD_TYPE Debug)
    set(CMAKE_CXX_FLAGS "-O0 -pipe -Og -ggdb -Wall -Wextra -fno-omit-frame-pointer -fprofile-arcs -ftest-coverage")
elseif (BUILD_RELEASE)
    set(CMAKE_BUILD_TYPE Release)
    set(CMAKE_CXX_FLAGS "-O3 -pipe -fPIC -flto -fno-fat-lto-objects")
else ()
    message(FATAL_ERROR "undefined mode.")
endif ()

add_library(protobuf_zmq STATIC
        protobuf.cpp
        protobuf_zmq.h
        )
# Anyway, we need to build ros_proto
add_subdirectory(ros_proto)
if (BUILD_DEBUG)
    add_subdirectory(test)
endif ()

install(TARGETS protobuf_zmq DESTINATION /usr/lib)
install(FILES ${PROJECT_SOURCE_DIR}/protobuf_zmq.h DESTINATION /usr/include)

# Build Static Lib

# Install source file