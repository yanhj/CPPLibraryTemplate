//********************************************************
/// @brief 
/// @author yanhuajian
/// @date 2023/6/1
/// @note
/// @version 1.0.0
//********************************************************

#pragma once

#define EXAMPLE_LOG_TAG "example"
#if defined(EXAMPLE_PLATFORM_ANDROID)
#       include <android/log.h>
#       define LOGV(...) { __android_log_print(ANDROID_LOG_VERBOSE,  EXAMPLE_LOG_TAG, __VA_ARGS__); }
#       define LOGD(...) { __android_log_print(ANDROID_LOG_DEBUG,    EXAMPLE_LOG_TAG, __VA_ARGS__); }
#       define LOGI(...) { __android_log_print(ANDROID_LOG_INFO,     EXAMPLE_LOG_TAG, __VA_ARGS__); }
#       define LOGW(...) { __android_log_print(ANDROID_LOG_WARN,     EXAMPLE_LOG_TAG, __VA_ARGS__); }
#       define LOGE(...) { __android_log_print(ANDROID_LOG_ERROR,    EXAMPLE_LOG_TAG, __VA_ARGS__); }
#       define LOGF(...) { __android_log_print(ANDROID_LOG_FATAL,    EXAMPLE_LOG_TAG, __VA_ARGS__); }
#else
#       define LOGV(...) { printf("<[%s] VERBOSE:> ", EXAMPLE_LOG_TAG);  printf(__VA_ARGS__); printf("\n"); }
#       define LOGD(...) { printf("<[%s]   DEBUG:> ", EXAMPLE_LOG_TAG);  printf(__VA_ARGS__); printf("\n"); }
#       define LOGI(...) { printf("<[%s]    INFO:> ", EXAMPLE_LOG_TAG);  printf(__VA_ARGS__); printf("\n"); }
#       define LOGW(...) { printf("<[%s]    WARN:> ", EXAMPLE_LOG_TAG);  printf(__VA_ARGS__); printf("\n"); }
#       define LOGE(...) { printf("<[%s]   ERROR:> ", EXAMPLE_LOG_TAG);  printf(__VA_ARGS__); printf("\n"); }
#       define LOGF(...) { printf("<[%s]   FATAL:> ", EXAMPLE_LOG_TAG);  printf(__VA_ARGS__); printf("\n"); }
#endif

#include <cstdio>

