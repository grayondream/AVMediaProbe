
set(MAIN_EXE_FILES ${SOURCE_DIR}/main.cpp)
set(UI_DIR ${SOURCE_DIR}/ui)
set(UTILS_DIR ${SOURCE_DIR}/utils)

source_group("src/" FILES ${MAIN_EXE_FILES})

include(${CMAKE_SOURCE_DIR}/src/env.cmake)
message(STATUS "${QT_PATH}")

if("${QT_PATH}" STREQUAL "")
    message(FATAL_ERROR "the qt path is empty, please set the path into your local Qt path or set env")
endif()

set(CMAKE_PREFIX_PATH ${QT_PATH})

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

get_target_property(QT_EXE_QMAKE Qt5::qmake IMPORTED_LOCATION)
get_filename_component(QT_BIN_DIR "${QT_EXE_QMAKE}" DIRECTORY)
find_program(QT_EXE_DEPLOYQT NAMES windeployqt macdeployqt HINTS "${QT_BIN_DIR}")
if("${QT_EXE_DEPLOYQT}" STREQUAL "")
    message(FATAL_ERROR "can not find qt deploy executable file in ${QT_BIN_DIR}")
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    add_compile_definitions(DEBUG=0)
else()
    add_compile_definitions(DEBUG=1)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(BINARY_DIR ${PROJECT_BINARY_DIR}/src/${CMAKE_BUILD_TYPE})
set(CONFIG_DIR ${BINARY_DIR}/config)

make_directory(${CONFIG_DIR})

message(STATUS "build type is ${CMAKE_BUILD_TYPE}")

message(${SOURCE_DIR})
file(GLOB SOURCE_DIR_FILES "${SOURCE_DIR}" *)
message(STATUS "the path is ${SOURCE_DIR_FILES}")
foreach(var ${SOURCE_DIR_FILES})
    if("${var}" STREQUAL "${SOURCE_DIR}")
        continue()
    endif()
    if(IS_DIRECTORY "${var}")
        include_directories(${var})
        file(GLOB G_INC_FILES LIST_DIRECTORIES false ${var} ${var}/*.h)
        file(GLOB G_SRC_FILES LIST_DIRECTORIES false ${var} ${var}/*.cpp)
        set(MAIN_EXE_FILES ${MAIN_EXE_FILES} ${G_INC_FILES} ${G_SRC_FILES})
        get_filename_component(G_NAME ${var} NAME_WE)
        source_group("src/${G_NAME}/include" FILES ${G_INC_FILES})
        source_group("src/${G_NAME}/src" FILES ${G_SRC_FILES})
    endif()
endforeach()

message(${FFMPEG_DYLIB_FILES})
foreach(var ${FFMPEG_DYLIB_FILES})
    file(COPY ${var} DESTINATION ${BINARY_DIR})
endforeach()

add_executable(${CMAKE_PROJECT_NAME} ${MAIN_EXE_FILES})
target_link_libraries(${CMAKE_PROJECT_NAME} Qt5::Core Qt5::Gui Qt5::Widgets)
target_link_libraries(${CMAKE_PROJECT_NAME} ${FFMPEG_LIB_FILES})

if(MSVC)
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE /WX /MP)
endif()

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND ${QT_EXE_DEPLOYQT} "$<TARGET_FILE:${CMAKE_PROJECT_NAME}>" --$<LOWER_CASE:$<CONFIG>>)