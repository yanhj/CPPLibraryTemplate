//********************************************************
/// @brief 
/// @author yanhuajian
/// @date 2023/6/1
/// @note
/// @version 1.0.0
//********************************************************

#pragma once

#include "LogLevelDefine.h"
#include <functional>
#include "Define.h"
#include <string>

NS_CUSTOM_BEGIN
    typedef std::function<void(LogLevel level, char *data)> LogCallBackFunc;
    extern LogCallBackFunc g_logCallback;
    extern std::string g_logTag;
    extern LogLevel g_logLevel;

    std::string logLevel2Str(LogLevel level);
NS_CUSTOM_END

#define MT_TARGET_LOG_LEVEL NS_CUSTOM::g_logLevel

#if defined(PLATFORM_OSX)
#define DEFAULT_LOG_BUFFER_SIZE 1024

#define  LOGV(...)  { \
    do { \
        if(MT_TARGET_LOG_LEVEL <= NS_CUSTOM::LogLevel::kLogVerbose) { \
            if(NS_CUSTOM::g_logCallback) { \
                char logBuffer[DEFAULT_LOG_BUFFER_SIZE]; \
                snprintf(logBuffer, DEFAULT_LOG_BUFFER_SIZE, __VA_ARGS__); \
                NS_CUSTOM::g_logCallback(NS_CUSTOM::LogLevel::kLogInfo, logBuffer); \
            } else { \
                printf("[%s][Verbose]: ", NS_CUSTOM::g_logTag.c_str());printf(__VA_ARGS__);printf("\n"); \
            } \
        } \
    } while(0); \
}

#define  LOGD(...) { \
    do { \
        if(MT_TARGET_LOG_LEVEL <= NS_CUSTOM::LogLevel::kLogDebug) { \
            if(NS_CUSTOM::g_logCallback) { \
                char logBuffer[DEFAULT_LOG_BUFFER_SIZE]; \
                snprintf(logBuffer, DEFAULT_LOG_BUFFER_SIZE, __VA_ARGS__); \
                NS_CUSTOM::g_logCallback(NS_CUSTOM::LogLevel::kLogDebug, logBuffer); \
            } else { \
                printf("[%s][Debug]: ", NS_CUSTOM::g_logTag.c_str());printf(__VA_ARGS__);printf("\n"); \
            } \
        } \
    } while(0); \
}

#define  LOGI(...)  { \
    do { \
        if(MT_TARGET_LOG_LEVEL <= NS_CUSTOM::LogLevel::kLogInfo) { \
            if (NS_CUSTOM::g_logCallback) { \
                char logBuffer[DEFAULT_LOG_BUFFER_SIZE]; \
                snprintf(logBuffer, DEFAULT_LOG_BUFFER_SIZE, __VA_ARGS__); \
                NS_CUSTOM::g_logCallback(NS_CUSTOM::LogLevel::kLogInfo, logBuffer); \
            } else { \
                printf("[%s][Info]: ", NS_CUSTOM::g_logTag.c_str());printf(__VA_ARGS__);printf("\n"); \
            } \
        } \
    } while(0); \
}

#define  LOGW(...) { \
    do { \
        if(MT_TARGET_LOG_LEVEL <= NS_CUSTOM::LogLevel::kLogWarning) { \
            if(NS_CUSTOM::g_logCallback) { \
                char logBuffer[DEFAULT_LOG_BUFFER_SIZE]; \
                snprintf(logBuffer, DEFAULT_LOG_BUFFER_SIZE, __VA_ARGS__); \
                NS_CUSTOM::g_logCallback(NS_CUSTOM::LogLevel::kLogWarning, logBuffer); \
            } else { \
                printf("[%s][Warning]: ", NS_CUSTOM::g_logTag.c_str());printf(__VA_ARGS__);printf("\n"); \
            } \
        } \
    } while(0); \
}

#define  LOGE(...)  { \
    do { \
        if(MT_TARGET_LOG_LEVEL <= NS_CUSTOM::LogLevel::kLogError) { \
            if(NS_CUSTOM::g_logCallback) { \
                char logBuffer[DEFAULT_LOG_BUFFER_SIZE]; \
                snprintf(logBuffer, DEFAULT_LOG_BUFFER_SIZE, __VA_ARGS__); \
                NS_CUSTOM::g_logCallback(NS_CUSTOM::LogLevel::kLogFatal, logBuffer); \
            } else { \
                printf("[%s][Error]: ", NS_CUSTOM::g_logTag.c_str());printf(__VA_ARGS__);printf("\n"); \
            } \
        } \
    } while(0); \
}

#define  LOGF(...)  { \
    do { \
        if(MT_TARGET_LOG_LEVEL <= NS_CUSTOM::LogLevel::kLogFatal) { \
            if(NS_CUSTOM::g_logCallback) { \
                char logBuffer[DEFAULT_LOG_BUFFER_SIZE]; \
                snprintf(logBuffer, DEFAULT_LOG_BUFFER_SIZE, __VA_ARGS__); \
                NS_CUSTOM::g_logCallback(NS_CUSTOM::LogLevel::kLogError, logBuffer); \
            } else { \
                printf("[%s][Fatal]: ", NS_CUSTOM::g_logTag.c_str());printf(__VA_ARGS__);printf("\n"); \
            } \
        } \
    } while(0); \
}



#elif defined(PLATFORM_WINDOWS)

#include <windows.h>
#define DEFAULT_LOG_BUFFER_SIZE 1024
#define LOGV(...) { \
    if(MT_TARGET_LOG_LEVEL <= NS_CUSTOM::LogLevel::kLogVerbose) { \
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); \
        if(NS_CUSTOM::g_logCallback) { \
            char logBuffer[DEFAULT_LOG_BUFFER_SIZE]; \
            snprintf(logBuffer, DEFAULT_LOG_BUFFER_SIZE, __VA_ARGS__); \
            NS_CUSTOM::g_logCallback(NS_CUSTOM::LogLevel::kLogVerbose, logBuffer); \
        } else {          \
            fprintf(stderr,"[%s][Verbose]: ", NS_CUSTOM::g_logTag.c_str());fprintf(stderr,__VA_ARGS__);fprintf(stderr,"\r\n"); \
        }               \
    }                   \
}
#define LOGD(...) { \
    if(MT_TARGET_LOG_LEVEL <= NS_CUSTOM::LogLevel::kLogDebug) { \
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN | FOREGROUND_BLUE |  FOREGROUND_INTENSITY); \
        if(NS_CUSTOM::g_logCallback) { \
            char logBuffer[DEFAULT_LOG_BUFFER_SIZE]; \
            snprintf(logBuffer, DEFAULT_LOG_BUFFER_SIZE, __VA_ARGS__); \
            NS_CUSTOM::g_logCallback(NS_CUSTOM::LogLevel::kLogDebug, logBuffer); \
        } else {          \
            fprintf(stderr,"[%s][Debug]: ", NS_CUSTOM::g_logTag.c_str());fprintf(stderr,__VA_ARGS__);fprintf(stderr,"\r\n"); \
        } \
    } \
}
#define LOGI(...) { \
    if(MT_TARGET_LOG_LEVEL <= NS_CUSTOM::LogLevel::kLogInfo) { \
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); \
        if(NS_CUSTOM::g_logCallback) { \
            char logBuffer[DEFAULT_LOG_BUFFER_SIZE]; \
            snprintf(logBuffer, DEFAULT_LOG_BUFFER_SIZE, __VA_ARGS__); \
            NS_CUSTOM::g_logCallback(NS_CUSTOM::LogLevel::kLogInfo, logBuffer); \
        } else {          \
            fprintf(stderr,"[%s][Info]: ", NS_CUSTOM::g_logTag.c_str());fprintf(stderr,__VA_ARGS__);fprintf(stderr,"\r\n"); \
        } \
    } \
}
#define LOGW(...) { \
    if(MT_TARGET_LOG_LEVEL <= NS_CUSTOM::LogLevel::kLogWarning) { \
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN | FOREGROUND_BLUE |  FOREGROUND_INTENSITY); \
        if(NS_CUSTOM::g_logCallback) { \
            char logBuffer[DEFAULT_LOG_BUFFER_SIZE]; \
            snprintf(logBuffer, DEFAULT_LOG_BUFFER_SIZE, __VA_ARGS__); \
            NS_CUSTOM::g_logCallback(NS_CUSTOM::LogLevel::kLogWarning, logBuffer); \
        } else {          \
            fprintf(stderr,"[%s][Warning]: ", NS_CUSTOM::g_logTag.c_str());fprintf(stderr,__VA_ARGS__);fprintf(stderr,"\r\n"); \
        } \
    } \
}
#define LOGE(...) { \
    if(MT_TARGET_LOG_LEVEL <= NS_CUSTOM::LogLevel::kLogError) { \
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED |  FOREGROUND_INTENSITY); \
        if(NS_CUSTOM::g_logCallback) { \
            char logBuffer[DEFAULT_LOG_BUFFER_SIZE]; \
            snprintf(logBuffer, DEFAULT_LOG_BUFFER_SIZE, __VA_ARGS__); \
            NS_CUSTOM::g_logCallback(NS_CUSTOM::LogLevel::kLogError, logBuffer); \
        } else { \
            fprintf(stderr,"[%s][Error]: ", NS_CUSTOM::g_logTag.c_str());fprintf(stderr,__VA_ARGS__);fprintf(stderr,"\r\n%s(%d)\r\n",__FILE__,__LINE__); \
        } \
    } \
}
#define LOGF(...) { \
    if(MT_TARGET_LOG_LEVEL <= NS_CUSTOM::LogLevel::kLogFatal) { \
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED |  FOREGROUND_INTENSITY); \
        if (NS_CUSTOM::g_logCallback) { \
            char logBuffer[DEFAULT_LOG_BUFFER_SIZE]; \
            snprintf(logBuffer, DEFAULT_LOG_BUFFER_SIZE, __VA_ARGS__); \
            NS_CUSTOM::g_logCallback(NS_CUSTOM::LogLevel::kLogFatal, logBuffer); \
        } else {          \
            fprintf(stderr,"[%s][Fatal]: ", NS_CUSTOM::g_logTag.c_str());fprintf(stderr,__VA_ARGS__);fprintf(stderr,"\r\n%s(%d)\r\n",__FILE__,__LINE__); \
        } \
    } \
}

#else // PLATFORM_UNKNOWN

#define  LOGV(...)
#define  LOGD(...)
#define  LOGI(...)
#define  LOGW(...)
#define  LOGE(...)
#define  LOGF(...)

#endif //
#define LOG_START_FUNC() LOGD("++(%s)++",__FUNCTION__)

#define LOG_END_FUNC() LOGD("--(%s)--",__FUNCTION__)

#include <stdio.h>
#include <ctime>




#define MACRO_LOG_RAII(X) \
class LOG##X##RAII{       \
public:                    \
    explicit LOG##X##RAII(const std::string & prefix= "",const std::string & content = ""); \
    ~LOG##X##RAII();      \
    void setContent(const std::string & content);  \
private: \
    std::string m_prefix; \
    std::string m_content;\
};



    MACRO_LOG_RAII(D) // LOGD_RAII
    MACRO_LOG_RAII(E) // LOGE_RAII
    MACRO_LOG_RAII(I) // LOGI_RAII
    MACRO_LOG_RAII(W) // LOGW_RAII

#define LOGD_RAII(PREFIX,CONTENT) \
    LOGDRAII _logDRAII(PREFIX,CONTENT);

#define LOGE_RAII(PREFIX,CONTENT) \
    LOGERAII _logERAII(PREFIX,CONTENT);
#define LOGI_RAII(PREFIX,CONTENT) \
    LOGIRAII _logIRAII(PREFIX,CONTENT);
#define LOGW_RAII(PREFIX,CONTENT) \
    LOGWRAII _logWRAII(PREFIX,CONTENT);

#undef MACRO_LOG_RAII





