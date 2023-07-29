set(THD_SPDLOG_DIR ${3PARTY_DIR}/spdlog)

message(STATUS "dependency spdlog location ${THD_SPDLOG_DIR}")
message(STATUS "include directory ${THD_SPDLOG_DIR}/include")

include_directories(${THD_SPDLOG_DIR}/include)