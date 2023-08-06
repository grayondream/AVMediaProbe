set(THD_FFMPEG_DIR ${3PARTY_DIR}/ffmpeg)

include_directories(${THD_FFMPEG_DIR}/include)

IF (WIN32)
    file(GLOB FFMPEG_LIB_FILES ${THD_FFMPEG_DIR}/lib/*.lib)
    file(GLOB FFMPEG_DYLIB_FILES ${THD_FFMPEG_DIR}/bin/*.dll)
ELSEIF (APPLE)
    file(GLOB FFMPEG_LIB_FILES ${THD_FFMPEG_DIR}/lib/*.dylib)
    file(GLOB FFMPEG_DYLIB_FILES ${THD_FFMPEG_DIR}/lib/*.dylib)
ELSEIF (UNIX)
    file(GLOB FFMPEG_LIB_FILES ${THD_FFMPEG_DIR}/lib/*.so)
    file(GLOB FFMPEG_DYLIB_FILES ${THD_FFMPEG_DIR}/lib/*.so)
ENDIF ()