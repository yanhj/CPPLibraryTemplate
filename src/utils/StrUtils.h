//********************************************************
/// @brief 
/// @author yanhuajian
/// @date 2023/12/6
/// @note
/// @version 1.0.0
//********************************************************

#pragma once

#include "Define.h"
#include <string>

NS_CUSTOM_BEGIN

namespace StrUtils {
    //字符串斜杠转换[默认将反斜杠转换为正斜杠]
    std::string slashConvert(const std::string& str, char slash = '/');
    //提取字符串文件名，不包含后缀
    std::string extractFileName(const std::string& str, bool withSuffix = false);
    //wchat_t转换为string
    std::string wstringToString(const std::wstring& wstr);
    std::string wcharToString(const wchar_t* wcstr);
    //string转换为wstring
    std::wstring stringToWstring(const std::string& str);

    bool startWith(const std::string& str, const std::string& prefix, bool bCaseSensitive = true);
    bool endWith(const std::string& str, const std::string& suffix, bool bCaseSensitive = true);

    bool isEqual(const std::string& str1, const std::string& str2, bool bCaseSensitive = true);
}

NS_CUSTOM_END