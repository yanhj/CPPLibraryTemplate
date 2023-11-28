//********************************************************
/// @brief 
/// @author yanhuajian
/// @date 2023/6/1
/// @note
/// @version 1.0.0
//********************************************************

#pragma once

////////// platform & export //////////
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(WIN64) || defined(_WIN64_) || defined(_WIN64)
#   ifdef EXAMPLE_STATIC
#       define EXAMPLE_API
#   else
#       ifdef EXAMPLE_BUILD
#           define EXAMPLE_API __declspec(dllexport)
#       else
#           define EXAMPLE_API __declspec(dllimport)
#       endif
#   endif
#else
#   if !defined(EXAMPLE_STATIC) && defined(EXAMPLE_BUILD)
#       define EXAMPLE_API __attribute__((visibility("default")))
#   else
#       define EXAMPLE_API
#   endif
#endif

// 优先使用CMAKE工程预定义平台
#if defined(CMAKE_PLATFORM_WINDOWS)
#define PLATFORM_WINDOWS 1
#elif defined(CMAKE_PLATFORM_OSX)
#define PLATFORM_OSX 1
#elif defined(CMAKE_PLATFORM_LINUX)
#define PLATFORM_LINUX 1
#elif defined(CMAKE_PLATFORM_IOS)
#define PLATFORM_IOS 1
#elif defined(CMAKE_PLATFORM_ANDROID)
#define PLATFORM_ANDROID 1
#elif defined(CMAKE_PLATFORM_EMSCRIPTEN)
#define PLATFORM_EMSCRIPTEN 1
#elif defined(_WIN32) || defined(_WIN32_) || defined(WIN32) || defined(_WIN64_) || defined(WIN64) || defined(_WIN64)
#define PLATFORM_WINDOWS 1
#elif defined(__APPLE__)
#include <TargetConditionals.h>
#if TARGET_OS_IPHONE || TARGET_OS_IOS
#define PLATFORM_IOS 1
#elif TARGET_OS_OSX
#define PLATFORM_OSX 1
#endif
#elif defined(ANDROID) || defined(_ANDROID_)
#define PLATFORM_ANDROID 1
#elif defined(__EMSCRIPTEN__)
    #define PLATFORM_EMSCRIPTEN 1
#elif defined(__linux__)
    #define PLATFORM_LINUX 1
#else
    #define PLATFORM_UNKNOWN 1
#endif


#ifdef __cplusplus
#  define EXAMPLE_API_C extern "C"
#else
#  define EXAMPLE_API_C
#endif

#define NS_CUSTOM custom_lib
#define NS_CUSTOM_BEGIN namespace NS_CUSTOM {
#define NS_CUSTOM_END }