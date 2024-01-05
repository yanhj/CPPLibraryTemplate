//********************************************************
/// @brief 
/// @author yanhuajian
/// @date 2023/12/22
/// @note
/// @version 1.0.0
//********************************************************

#include "PathUtils.h"
#include <string>
#include "log/log.h"
#include <vector>
#include "utils/StrUtils.h"
#include <algorithm>
#ifdef _WIN32
#include <direct.h>
#include <corecrt_io.h>

#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <cstring>
#endif
#include <queue>

static bool mtlab_file_exist(const char * __restrict __filenameUtf8) {
    bool bExist = false;
#if defined(_WIN32) || defined(_MSC_VER)
    bExist =  (_access(__filenameUtf8, 0) == 0);
#else
    bExist = (access(__filenameUtf8, 0) == 0);
#endif
    return bExist;
}

static bool autoCreateDir(char* pStart, char* pEnd) {
#if defined(__APPLE__)
    for (auto pPtr = pEnd - 1; pPtr != pStart; pPtr--) {
        if ((*pPtr) == '/' || (*pPtr) == '\\') {
            char c = (*pPtr);
            (*pPtr) = 0;
            bool bSuccess = (access(pStart, 0) == 0);
            if (!bSuccess) {
                bSuccess = autoCreateDir(pStart, pPtr);

                if (bSuccess) {
                    bSuccess = (mkdir(pStart, 0755) != -1);
                }
            }
            (*pPtr) = c;
            return bSuccess;
        }
    }
#elif defined(_WIN32)
    for (auto pPtr = pEnd - 1; pPtr != pStart; pPtr--) {
			if ((*pPtr) == '/' || (*pPtr) == '\\') {
				char c = (*pPtr);
				(*pPtr) = 0;
				const char* pPath = pStart;

				bool bSuccess = mtlab_file_exist(pPath);
				if (!bSuccess) {
					bSuccess = autoCreateDir(pStart, pPtr);

					if (bSuccess) {
						bSuccess = (_mkdir(pPath) != -1);
					}
				}
				(*pPtr) = c;
				return bSuccess;
			}
		}
#endif
    return false;
}
NS_CUSTOM_BEGIN

namespace PathUtils {

    std::string executablePath(unsigned int depth) {
        std::string path;
    #ifdef __APPLE__
        unsigned int bufferSize = 512;
        std::vector<char> buffer(bufferSize + 1);
        memset(&buffer[0], 0, bufferSize + 1);
        // "_NSGetExecutablePath" will return "-1" if the buffer is not large enough
        // and "*bufferSize" will be set to the size required.
        if (_NSGetExecutablePath(&buffer[0], &bufferSize) != 0) {
            buffer.resize(bufferSize);
            _NSGetExecutablePath(&buffer[0], &bufferSize);
        }
        path = std::string(buffer.begin(), buffer.end());
    #elif defined(_WIN32)
        UINT bufferSize = MAX_PATH;
        std::vector<wchar_t> buffer(bufferSize + 1);
        const HMODULE hModule = GetModuleHandleW(nullptr);
        bufferSize = GetModuleFileNameW(hModule, &buffer[0], bufferSize);
        if (bufferSize > MAX_PATH) {
            buffer.resize(bufferSize);
            GetModuleFileNameW(hModule, &buffer[0], bufferSize);
        }
        wchar_t *lastBackslash = std::wcsrchr(&buffer[0], L'\\');
        if (lastBackslash == nullptr) {
            return {};
        }
        *lastBackslash = L'\0';
        path = StrUtils::wcharToString(&buffer[0]);
#elif defined(__linux__)
        char buffer[FILENAME_MAX] = {};
        memset(&buffer[0], 0, sizeof(buffer));
        const int bufferSize = sizeof(buffer);
        const int bytes = MIN(readlink("/proc/self/exe", buffer, bufferSize), (bufferSize - 1));
        if (bytes >= 0) {
            buffer[bytes] = '\0';
        }
        path = std::string(buffer.begin(), buffer.end());
#else
#endif
        char slash = '/';
#if defined(_WIN32)
        slash = '\\';
#endif
        while (depth > 0) {
            const size_t lastSlash = path.find_last_of(slash);
            if (lastSlash == std::string::npos) {
                return {};
            }
            path = path.substr(0, lastSlash);
            --depth;
        }
        return path;
    }

    bool isExist(const std::string& filePathUtf8) {
        return mtlab_file_exist(filePathUtf8.c_str());
    }

    bool checkAndCreateDir(const std::string &dirPathUtf8) {
        if (dirPathUtf8.empty()) {
            return false;
        }
        //判断是否存在
        if (isExist(dirPathUtf8)) {
            return true;
        }
        auto dirPath(dirPathUtf8);
        //判断尾部是否是斜杠
        if (dirPath.back() != '/' && dirPath.back() != '\\') {
            dirPath.append("/");
        }
        auto cStr = dirPath.c_str();
        return autoCreateDir(const_cast<char*>(cStr), const_cast<char*>(cStr) + dirPath.size());
    }

    bool removeFile(const std::string &filePathUtf8) {
        if (filePathUtf8.empty()) {
            return false;
        }
#if defined(_WIN32) || defined(_MSC_VER)
        if (DeleteFileA(filePathUtf8.c_str()) != 0) {
            return true;
        }
#elif defined(__APPLE__)
        if (unlink(filePathUtf8.c_str()) == 0) {
            return true;
        }
#endif
        return false;
    }

    bool moveFile(const std::string &src, const std::string &dst) {
#ifdef _WIN32
        if (MoveFileExA(src.c_str(), dst.c_str(), MOVEFILE_REPLACE_EXISTING) != 0) {
            return true;
        }
#elif defined(__APPLE__)
        if (rename(src.c_str(), dst.c_str()) == 0) {
            return true;
        }
#endif
        return false;
    }

    std::string dirPath(const std::string &filePath) {
        if (filePath.empty()) {
            return {};
        }
        //如果尾部是斜杠，去掉
        auto path = filePath;
        while (path.back() == '/' || path.back() == '\\') {
            path = path.substr(0, path.size() - 1);
        }
        const size_t lastForwardSlash = filePath.find_last_of('/');
        const size_t lastBackSlash = filePath.find_last_of('\\');
        size_t lastSlash = lastForwardSlash;
        if (lastBackSlash != std::string::npos) {
            lastSlash = (std::max)(lastForwardSlash, lastBackSlash);
        }
        if (lastSlash == std::string::npos) {
            //未找到斜杠
            return {};
        }
        return filePath.substr(0, lastSlash);
    }

    std::string toLocalPath(const std::string& utf8Path) {
        std::string strResult(utf8Path);
#ifdef _WIN32
        std::wstring strUtf16;
    int nWCSize = MultiByteToWideChar(CP_UTF8, 0, utf8Path.c_str(), -1, 0, 0);
    if (nWCSize) {
        strUtf16.resize(nWCSize, 0);
        MultiByteToWideChar(CP_UTF8, 0, utf8Path.c_str(), -1, &strUtf16[0], nWCSize);
    }
    int nMBSize = WideCharToMultiByte(0, 0, strUtf16.c_str(), -1, 0, 0, 0, 0);
    if (nMBSize) {
        strResult.resize(nMBSize, 0);
        WideCharToMultiByte(0, 0, strUtf16.c_str(), -1, &strResult[0], nMBSize, 0, 0);
    }
    return strResult;
#endif
        return strResult;
    }

    bool copyFile(const std::string &src, const std::string &dst) {
#if defined(_WIN32) || defined(_MSC_VER)
        if (CopyFileA(src.c_str(), dst.c_str(), FALSE) != 0) {
            return true;
        }
#elif defined(__APPLE__)
        FILE* sourceFile = fopen(src.c_str(), "rb");
        FILE* destinationFile = fopen(dst.c_str(), "wb");

        if (sourceFile && destinationFile) {
            char buffer[4096];
            size_t bytesRead;

            while ((bytesRead = fread(buffer, 1, sizeof(buffer), sourceFile)) > 0) {
                fwrite(buffer, 1, bytesRead, destinationFile);
            }

            fclose(sourceFile);
            fclose(destinationFile);
            return true;
        }
#endif
        return false;
    }

    int getFiles(const std::string &dirPath, std::vector<std::string> &files) {
        if (dirPath.empty()) {
            return 0;
        }
        //判断是否存在
        if (!isExist(dirPath)) {
            return 0;
        }

        //定义一个文件夹路径队列
        std::queue<std::string> dirQueue;
        dirQueue.push(dirPath);
        while (!dirQueue.empty()) {
#if defined(_WIN32) || defined(_MSC_VER)
            //获取文件夹下的所有文件
            std::string ditTmp = dirQueue.front();
            dirQueue.pop();
            std::string strFind = ditTmp + "\\*.*";
            intptr_t hFile = 0;
            _finddata_t fileinfo;
            if ((hFile = _findfirst(strFind.c_str(), &fileinfo)) != -1) {
                do {
                    if ((fileinfo.attrib & _A_SUBDIR)) {
                        //是文件夹
                        if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {
                            dirQueue.push(ditTmp + "\\" + fileinfo.name);
                        }
                    } else {
                        //是文件
                        files.push_back(ditTmp + "\\" + fileinfo.name);
                    }
                } while (_findnext(hFile, &fileinfo) == 0);
                _findclose(hFile);
            }
#elif defined(__APPLE__)
            //取出队列中的第一个文件夹路径
            std::string dirTmp = dirQueue.front();
            dirQueue.pop();
            // 打开目录
            DIR* dir = opendir(dirTmp.c_str());
            if (dir == nullptr) {
                break;
            }
            // 读取目录中的文件和文件夹
            dirent *entry = nullptr;
            while ((entry = readdir(dir)) != nullptr) {
                // 过滤掉当前目录 "." 和上级目录 ".."
                if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                    continue;
                }
                //判断是否是文件夹
                if (entry->d_type == DT_DIR) {
                    dirQueue.push(dirTmp + "/" + entry->d_name);
                } else if (entry->d_type == DT_REG) {
                    //是文件
                    files.push_back(dirTmp + "/" + entry->d_name);
                }
            }
            // 关闭目录
            closedir(dir);
#endif
        }
        return files.size();
    }
} // namespace PathUtils

NS_CUSTOM_END
