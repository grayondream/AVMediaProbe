project(AVMediaProbe)
cmake_minimum_required(VERSION 3.10)

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(CMAKE_CXX_STANDARD 17)

set(3PARTY_DIR ${CMAKE_SOURCE_DIR}/thirdparty)

include(${3PARTY_DIR}/spdlog.cmake)
include(${3PARTY_DIR}/ffmpeg.cmake)

add_subdirectory(${SOURCE_DIR})
