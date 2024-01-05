//********************************************************
/// @brief 
/// @author yanhuajian
/// @date 2023/12/21
/// @note
/// @version 1.0.0
//********************************************************

#pragma once

#include "Define.h"
#include <string>

NS_CUSTOM_BEGIN

    class File {
    public:
        File() = default;
        explicit File(const std::string &path);
        ~File();

        bool open(const std::string& mode);

        std::string readAll();

        void write(const std::string &data);
        void close();
        long size();

        //判断文件是否存在
        bool isExist() const;
        static bool isExist(const std::string& path);

        //删除文件
        bool remove();
        static bool remove(const std::string& path);

        //移动文件
        bool move(const std::string& dst);
        static bool move(const std::string& src, const std::string& dst);

        //拷贝文件
        bool copy(const std::string& dst);
        static bool copy(const std::string& src, const std::string& dst);

        //获取文件所在目录
        std::string dir() const;
        bool mkdir() const;

        //获取文件名
        std::string name() const;
        std::string path() const;

    private:
        std::string m_path;
        FILE* m_fp = nullptr;
        bool m_bOpened = false;
    };

NS_CUSTOM_END