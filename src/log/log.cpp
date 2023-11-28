//********************************************************
/// @brief
/// @author yanhuajian
/// @date 2023/11/27
/// @note
/// @version 1.0.0
//********************************************************

#include "log.h"

NS_CUSTOM_BEGIN
    LogCallBackFunc g_logCallback = nullptr;
    std::string g_logTag = "CustomLib";
    LogLevel g_logLevel = kLogVerbose;

    std::string logLevel2Str(LogLevel level) {
        switch (level) {
            case kLogVerbose:
                return "Verbose";
            case kLogDebug:
                return "Debug";
            case kLogInfo:
                return "Info";
            case kLogWarning:
                return "Warning";
            case kLogError:
                return "Error";
            case kLogFatal:
                return "Fatal";
            default:
                return "Unknown";
        }
    }

NS_CUSTOM_END