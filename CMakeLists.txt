project(AVMediaProbe)
cmake_minimum_required(VERSION 3.10)

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(CMAKE_CXX_STANDARD 17)

set(3PARTY_DIR ${CMAKE_SOURCE_DIR}/thirdparty)

file(GLOB 3PARTY_LIST ${3PARTY_DIR} ${3PARTY_DIR}/*.cmake)

foreach(var ${3PARTY_LIST})
    if("${var}" STREQUAL "${3PARTY_DIR}")
        continue()
    endif()
    message(STATUS "include ${var}")
    include(${var})
endforeach(var)

add_subdirectory(${SOURCE_DIR})
