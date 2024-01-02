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
    class CUSTOM_API Log {
    public:
        static void setLogCallback(std::function<void(LogLevel level, const char *log)> logCallback);
        static void setLogLevel(LogLevel level);
    };
NS_CUSTOM_END