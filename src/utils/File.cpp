//********************************************************
/// @brief 
/// @author yanhuajian
/// @date 2023/12/21
/// @note
/// @version 1.0.0
//********************************************************

#include "File.h"
#include "log/log.h"
#include "PathUtils.h"
#include "StrUtils.h"

NS_CUSTOM_BEGIN

    File::File(const std::string &path)
    : m_path(path) {
    }

    File::~File() {
       close();
    }

    std::string File::readAll() {
        int fileSize = size() + 1;
        //读取文件
        auto buffer = new char[fileSize];
        std::fread(buffer, fileSize, 1, m_fp);
        buffer[fileSize - 1] = 0;
        std::string str(buffer);
        delete [] buffer;
        return str;
    }

    long File::size() {
        if (!m_fp) {
            return -1;
        }
        // file指针指向文件尾
        std::fseek(m_fp, 0, SEEK_END);
        long iSize = ftell(m_fp);
        std::fseek(m_fp, 0, SEEK_SET);

        return iSize;
    }

    bool File::open(const std::string& mode) {
        if (m_bOpened) {
            return true;
        }
        //打开文件
        m_fp = std::fopen(m_path.c_str(), mode.c_str());
        if(nullptr == m_fp) {
            LOGE("File opening failed");
            m_bOpened = false;
            return false;
        }
        m_bOpened = true;
        return true;
    }

    void File::write(const std::string &data) {
        if (!m_fp) {
            return;
        }
        std::fwrite(data.c_str(), data.size(), 1, m_fp);
    }

    void File::close() {
        //关闭文件
        if (m_fp) {
            std::fclose(m_fp);
            m_fp = nullptr;
        }
    }

    bool File::isExist(const std::string &path) {
        return PathUtils::isExist(path);
    }

    bool File::isExist() const {
        return PathUtils::isExist(m_path);
    }

    bool File::remove() {
        return PathUtils::removeFile(m_path);
    }

    bool File::remove(const std::string &path) {
        return PathUtils::removeFile(path);
    }

    bool File::move(const std::string &dst) {
        return PathUtils::moveFile(m_path, dst);
    }

    bool File::move(const std::string &src, const std::string &dst) {
        return PathUtils::moveFile(src, dst);
    }

    std::string File::dir() const {
        return PathUtils::dirPath(m_path);
    }

    std::string File::name() const {
        return StrUtils::extractFileName(m_path);
    }

    std::string File::path() const {
        return m_path;
    }

    bool File::copy(const std::string &dst) {
        return PathUtils::copyFile(m_path, dst);
    }

    bool File::copy(const std::string &src, const std::string &dst) {
        return PathUtils::copyFile(src, dst);
    }

    bool File::mkdir() const {
        return PathUtils::checkAndCreateDir(dir());
    }

NS_CUSTOM_END