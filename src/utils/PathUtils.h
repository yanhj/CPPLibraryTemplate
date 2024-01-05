//********************************************************
/// @brief 
/// @author yanhuajian
/// @date 2023/12/22
/// @note
/// @version 1.0.0
//********************************************************

#pragma once

#include "Define.h"
#include <string>
#include <vector>

NS_CUSTOM_BEGIN

namespace PathUtils {
    /**
     * @brief 获取可执行文件路径
     * @param depth 0:可执行文件路径 1:可执行文件所在目录路径 2:可执行文件所在目录的上一级目录路径
     * @return
     */
    std::string executablePath(unsigned int depth = 1);

    /**
     * @brief 判断文件路径是否存在
     * @param filePathUtf8 文件路径
     * @return true:存在 false:不存在
     */
    bool isExist(const std::string& filePathUtf8);

    /**
     * @brief 创建文件夹
     * @param filePathUtf8 文件路径
     * @return true:存在 false:不存在
     */
    bool checkAndCreateDir(const std::string& dirPathUtf8);

    /**
     * @brief 删除文件
     * @param filePathUtf8 文件路径
     * @return true:存在 false:不存在
     */
    bool removeFile(const std::string& filePathUtf8);

    /**
     * @brief 移动文件
     * @param src 源文件路径
     * @param dst 目标文件路径
     * @return true:成功 false:失败
     */
    bool moveFile(const std::string& src, const std::string& dst);

    /**
     * @brief 拷贝文件
     * @param src 源文件路径
     * @param dst 目标文件路径
     */
     bool copyFile(const std::string& src, const std::string& dst);

    /**
     * @brief 获取文件夹路径
     * @param filePath 文件路径
     * @return 文件夹路径
     */
    std::string dirPath(const std::string& filePath);

    std::string toLocalPath(const std::string& utf8Path);

    /**
     * @brief 获取文件夹下的所有文件
     * @param dirPath 文件夹路径
     * @param files 文件列表
     * @return 文件数量
     */
    int getFiles(const std::string& dirPath, std::vector<std::string>& files);

};

NS_CUSTOM_END