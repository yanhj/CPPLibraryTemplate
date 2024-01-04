//********************************************************
/// @brief 
/// @author yanhuajian
/// @date 2023/1/11
/// @note
/// @version 1.0.0
//********************************************************
#pragma once

#include "Define.h"

NS_CUSTOM_BEGIN
    enum LogLevel {
        kLogAll	    = 0x0000,
        kLogVerbose = 0x0001,
        kLogDebug	= 0x0002,
        kLogInfo	= 0x0004,
        kLogWarning	= 0x0008,
        kLogError	= 0x0010,
        kLogFatal	= 0x0020,
        kLogOff		= 0xFFFF,
    };
NS_CUSTOM_END