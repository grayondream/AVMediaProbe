#pragma once

#include <cstdint>

enum class ErrorCode : int32_t {
    None,
    FAILED = -1,
	IO_FAILED = -2,
};