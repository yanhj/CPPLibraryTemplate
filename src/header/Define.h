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

#define NS_CUSTOM custom_lib
#define NS_CUSTOM_BEGIN namespace NS_CUSTOM {
#define NS_CUSTOM_END }