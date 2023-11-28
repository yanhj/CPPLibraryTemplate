//********************************************************
/// @brief 
/// @author yanhuajian
/// @date 2023/1/11
/// @note
/// @version 1.0.0
//********************************************************

#pragma once

#include <functional>
#include "LogLevelDefine.h"
#include "Define.h"

NS_CUSTOM_BEGIN
    EXAMPLE_API_C void setLogCallback(std::function<void(int level, const char *log)> logCallback);
    EXAMPLE_API_C void setLogLevel(LogLevel level);
NS_CUSTOM_END