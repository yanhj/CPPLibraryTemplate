//********************************************************
/// @brief 
/// @author yanhuajian
/// @date 2023/1/11
/// @note
/// @version 1.0.0
//********************************************************

#include "LogCallback.h"
#include "log.h"

//日志回调
NS_CUSTOM_BEGIN

    void Log::setLogCallback(std::function<void(LogLevel level, const char *log)> logCallback) {
        g_logCallback = logCallback;
    }

    void Log::setLogLevel(LogLevel level) {
        g_logLevel = level;
    }

NS_CUSTOM_END