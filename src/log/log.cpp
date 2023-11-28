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
    LogLevel g_mtDRLogLevel = kLogVerbose;
NS_CUSTOM_END