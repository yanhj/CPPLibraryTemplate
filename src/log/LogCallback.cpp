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

    void setLogCallback(std::function<void(LogLevel level, const char *log)> logCallback) {
        g_logCallback = logCallback;
    }

    void setLogLevel(LogLevel level) {
        g_mtDRLogLevel = level;
    }

NS_CUSTOM_END