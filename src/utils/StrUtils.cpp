//********************************************************
/// @brief 
/// @author yanhuajian
/// @date 2023/12/6
/// @note
/// @version 1.0.0
//********************************************************

#include "StrUtils.h"
#include "log/log.h"
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <cctype>
#include <algorithm>

NS_CUSTOM_BEGIN

std::string StrUtils::slashConvert(const std::string& str, char slash) {
    std::string result = str;
    if(slash == '/') {
        //将反斜杠转换为正斜杠
        std::replace(result.begin(), result.end(), '\\', '/');
        return result;
    }
    //将正斜杠转换为反斜杠
    std::replace(result.begin(), result.end(), '/', '\\');
    return result;
}

std::string StrUtils::extractFileName(const std::string& str, bool withSuffix) {
    std::string result = str;
    auto pos = result.find_last_of("/\\");
    if (pos != std::string::npos) {
        result = result.substr(pos + 1);
    }
    if (!withSuffix) {
        pos = result.find_last_of(".");
        if (pos != std::string::npos) {
            result = result.substr(0, pos);
        }
    }
    return result;
}

    std::string StrUtils::wstringToString(const std::wstring &wstr) {
        return wcharToString(wstr.c_str());
    }

    std::string StrUtils::wcharToString(const wchar_t *wcstr) {
#if defined(_WIN32) || defined(_MSC_VER)
        // Calculate the length of the multibyte string
        DWORD len = WideCharToMultiByte(CP_UTF8, 0, wcstr, -1, NULL, 0, NULL, NULL);
        if (len == static_cast<size_t>(-1)) {
            LOGW("std::wcstombs() Conversion failed");
            return "";
        }
        std::vector<char> mbstr(len, '\0');
        // Convert wide character string to multibyte string
        WideCharToMultiByte(CP_UTF8, 0, wcstr, -1, mbstr.data(), len, NULL, NULL);
        return std::string(mbstr.begin(), mbstr.end());
#else
        size_t len = wcstombs(NULL, wcstr, 0);
        if (len == static_cast<size_t>(-1)) {
            LOGW("std::wcstombs() Conversion failed");
            return "";
        }
        std::vector<char> mbstr(len, '\0');
        wcstombs(mbstr.data(), wcstr, len);
        return std::string(mbstr.begin(), mbstr.end());
#endif
    }

    std::wstring StrUtils::stringToWstring(const std::string &str) {
#if defined(_WIN32) || defined(_MSC_VER)
        DWORD len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
        if (len > 0) {
            std::vector<wchar_t> wPathBuf(len, '\0');
            if (MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, (LPWSTR)wPathBuf.data(), len) > 0) {
                return std::wstring(wPathBuf.begin(), wPathBuf.end());
            }
        }
#else
        size_t len = mbstowcs(NULL, str.c_str(), 0);
        if (len > 0) {
            std::vector<wchar_t> wPathBuf(len, '\0');
            if (mbstowcs(wPathBuf.data(), str.c_str(), len) > 0) {
                return std::wstring(wPathBuf.begin(), wPathBuf.end());
            }
        }
#endif
        return std::wstring();
    }

    bool StrUtils::startWith(const std::string &str, const std::string &prefix, bool bCaseSensitive) {
        auto len = prefix.size();
        return str.length() > len && isEqual(str.substr(0, len), prefix, bCaseSensitive);
    }

    bool StrUtils::endWith(const std::string &str, const std::string &suffix, bool bCaseSensitive) {
        auto len = suffix.size();
        auto totalLen = str.length();
        return totalLen > len && isEqual(str.substr(totalLen - len), suffix, bCaseSensitive);
    }

    bool compareCaseInsensitive(char a, char b) {
        return std::toupper(static_cast<unsigned char>(a)) == std::toupper(static_cast<unsigned char>(b));
    }

    bool StrUtils::isEqual(const std::string &str1, const std::string &str2, bool bCaseSensitive) {
        //比较字符串，不区分大小写
        if(bCaseSensitive) {
            return std::equal(str1.begin(), str1.end(), str2.begin());
        }
        // 自定义比较函数，不区分大小写
        return std::equal(str1.begin(), str1.end(), str2.begin(), compareCaseInsensitive);
    }

NS_CUSTOM_END