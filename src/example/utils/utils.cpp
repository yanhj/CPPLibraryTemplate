
#include <example/utils/utils.h>
#include <log/log.h>

// 平台宏定义
// 优先使用CMAKE工程预定义平台
#if defined(CMAKE_PLATFORM_WINDOWS)
#   define EXAMPLE_PLATFORM_WINDOWS 1
#elif defined(CMAKE_PLATFORM_OSX) || defined(CMAKE_PLATFORM_MACOS)
#   define EXAMPLE_PLATFORM_OSX 1
#   define EXAMPLE_PLATFORM_MACOS 1
#elif defined(CMAKE_PLATFORM_LINUX)
#   define EXAMPLE_PLATFORM_LINUX 1
#elif defined(CMAKE_PLATFORM_IOS)
#   define EXAMPLE_PLATFORM_IOS 1
#elif defined(CMAKE_PLATFORM_ANDROID)
#   define EXAMPLE_PLATFORM_ANDROID 1
#elif defined(CMAKE_PLATFORM_EMSCRIPTEN)
#   define EXAMPLE_PLATFORM_EMSCRIPTEN 1
#elif defined(_WIN32) || defined(_WIN32_) || defined(WIN32) || defined(_WIN64_) || defined(WIN64) || defined(_WIN64)
#   define EXAMPLE_PLATFORM_WINDOWS 1
#elif defined(__EMSCRIPTEN__)
#   define EXAMPLE_PLATFORM_EMSCRIPTEN 1
#elif defined(__APPLE__)
#   include <TargetConditionals.h>
#   if TARGET_OS_IPHONE
#       define EXAMPLE_PLATFORM_IOS 1
#   elif TARGET_OS_OSX
#       define EXAMPLE_PLATFORM_OSX 1
#       define EXAMPLE_PLATFORM_MACOS 1
#   else
#       error "Unknown platform"
#   endif
#elif defined(ANDROID) || defined(_ANDROID_)
#   define EXAMPLE_PLATFORM_ANDROID 1
#elif defined(__linux__)
#   define EXAMPLE_PLATFORM_LINUX 1
#else
#   define EXAMPLE_PLATFORM_UNKNOWN 1
#   error "Unknown platform"
#endif

#include <cstdio>
#include <cstdlib>
#include <string>
#include <thread>

#ifdef EXAMPLE_PLATFORM_ANDROID
#   include <sys/system_properties.h>
#endif

#ifndef EXAMPLE_PLATFORM_WINDOWS
#include <unistd.h>
#endif

#ifdef EXAMPLE_PLATFORM_IOS
#include <sys/sysctl.h>
#endif

#ifdef EXAMPLE_PLATFORM_LINUX
#include <cstring>
#endif

#define SYSTEM_CMD(cmd) (__system_cmd(cmd).c_str())
#ifdef EXAMPLE_PLATFORM_IOS
#define SYSCTLBYNAME_CMD(cmd, type) (__sysctlbyname(cmd, type).c_str())
#define SYSCTL_CMD(cmd) (__sysctl(cmd).c_str())
#endif


namespace example {

#ifdef EXAMPLE_PLATFORM_IOS

    static std::string __sysctlbyname(const char* cmd, int type = 0) {
        const static int buf_max = 1024;
        char buf[buf_max]={0};
        size_t size;
        sysctlbyname(cmd, buf, &size, NULL, 0);
        if (type == 0) {
            int buf_len = (int)strlen(buf);
            if (buf[buf_len-1] == '\n') {
                buf[buf_len-1] = '\0';
            }
        }
        else if (type == 1) {
            int t = *((int*)buf);
            sprintf(buf, "%d%c", t, '\0');
        }
        else if (type == 2) {
            size_t t = *((size_t*)buf);
            t /= 1024;
            size_t x = 1024*1024;
            t = (t+x-1)/x;
            sprintf(buf, "%zu GB%c", t, '\0');
        }
        return std::string(buf);
    }

    static std::string __sysctl(int typeSpecifier) {
        size_t size = sizeof(int);
        int results;
        int mib[2] = {CTL_HW, typeSpecifier};
        sysctl(mib, 2, &results, &size, NULL, 0);

        const static int buf_max = 1024;
        char buf[buf_max]={0};
        sprintf(buf, "%d%c", results, '\0');
        return std::string(buf);
    }
#endif

#if !defined(EXAMPLE_PLATFORM_IOS) && !defined(EXAMPLE_PLATFORM_WINDOWS)
    static std::string __system_cmd(const char* cmd) {
        const static int buf_max = 1024;
        char buf[buf_max]={0};
        int fd[2];
        int back_fd;
        pipe(fd);
        //备份标准输出，用于恢复
        back_fd = dup(STDOUT_FILENO);
        //将标准输出重定向到fd[1]
        dup2(fd[1],STDOUT_FILENO);
        int status = system(cmd);
        if (status == 0) {
            read(fd[0],buf,buf_max);
        }
        //恢复标准输出
        dup2(back_fd,STDOUT_FILENO);
        int buf_len = (int)strlen(buf);
        if (buf_len > 0) {
            if (buf[buf_len-1] == '\n') {
                buf[buf_len-1] = '\0';
            }
        }
        return std::string(buf);
    }
#endif

    void Utils::PrintSystem() {
        LOGV("Application        : %zu-bits", sizeof(void*)*8);
#ifdef EXAMPLE_PLATFORM_IOS
        LOGV("Machine            : %s",SYSCTLBYNAME_CMD("hw.machine", 0));
        LOGV("Memory             : %s",SYSCTLBYNAME_CMD("hw.memsize", 2));
        LOGV("CPU core           : %s",SYSCTL_CMD(HW_NCPU));
        LOGV("CPU core conf      : %d",(int)sysconf(_SC_NPROCESSORS_CONF));
        LOGV("CPU core online    : %d",(int)sysconf(_SC_NPROCESSORS_ONLN));
        LOGV("CPU thread std     : %u",std::thread::hardware_concurrency());
#endif
#ifdef EXAMPLE_PLATFORM_OSX
        LOGV("CPU type           : %s",SYSTEM_CMD("sysctl -n machdep.cpu.brand_string"));
        LOGV("CPU core           : %s",SYSTEM_CMD("sysctl -n machdep.cpu.core_count"));
        LOGV("CPU core conf      : %d",(int)sysconf(_SC_NPROCESSORS_CONF));
        LOGV("CPU core online    : %d",(int)sysconf(_SC_NPROCESSORS_ONLN));
        LOGV("CPU thread         : %s",SYSTEM_CMD("sysctl -n machdep.cpu.thread_count"));
        LOGV("CPU thread std     : %u",std::thread::hardware_concurrency());
        LOGV("OS                 : %s",SYSTEM_CMD("sw_vers -productName"));
        LOGV("Version            : %s",SYSTEM_CMD("sw_vers -productVersion"));
#endif
#ifdef EXAMPLE_PLATFORM_ANDROID
        {
            char value[93] = "";
            __system_property_get("ro.product.brand", value);
            LOGV("Device Brand   : %s",value);
        }
        {
            char value[93] = "";
            __system_property_get("ro.product.model", value);
            LOGV("Device Model   : %s",value);
        }
        {
            char value[93] = "";
            __system_property_get("ro.product.name", value);
            LOGV("Device Name    : %s",value);
        }
        {
            char value[93] = "";
            __system_property_get("ro.product.cpu.abi", value);
            LOGV("Device ABI     : %s",value);
        }
        {
            char value[93] = "";
            __system_property_get("ro.product.cpu.abilist", value);
            LOGV("Device ABI list: %s",value);
        }
        {
            std::string t(SYSTEM_CMD("cat /proc/cpuinfo | grep Hardware"));
            if (t.length() > 1) {
                size_t pos = std::string::npos;
                if ((pos = t.find(':')) != std::string::npos && pos < t.length()) {
                    t = t.substr(pos+1);
                }
                if ((pos = t.find_first_not_of(' ')) != std::string::npos) {
                    t = t.substr(pos);
                }
            }
            else {
                t = "Unknown";
                {
                    // HUAWEI
                    char value[93] = "";
                    __system_property_get("ro.product.vendor.name", value);
                    if (strlen(value) > 2) {
                        t = std::string(value);
                    }
                }
            }
            LOGV("CPU type       : %s",t.c_str());
        }
        {
            std::string t(SYSTEM_CMD("ls /sys/devices/system/cpu | grep cpu[0-9]"));
            int count = 0;
            if (t.length() > 1) {
                size_t pos = std::string::npos;
                while (true) {
                    if ((pos = t.find("cpu")) != std::string::npos) {
                        t = t.substr(pos + 3);
                        count ++;
                    }
                    else {
                        break;
                    }
                }
            }
            LOGV("CPU core       : %d",count);
            LOGV("CPU core conf  : %d",(int)sysconf(_SC_NPROCESSORS_CONF));
            LOGV("CPU core online: %d",(int)sysconf(_SC_NPROCESSORS_ONLN));
            LOGV("CPU thread std : %u",std::thread::hardware_concurrency());
        }
        {
            std::string t(SYSTEM_CMD("cat /proc/meminfo | grep MemTotal"));
            int tt = 0;
            if (t.length() > 1) {
                char buf[1024] = {0};
                int count = 0;
                for (auto& c : t) {
                    if (c >= '0' && c <= '9') {
                        buf[count++] = c;
                    }
                }
                buf[count++] = '\0';
                tt = atoi(buf);
                int x = 1024*1024;
                tt = (tt + x - 1) / x;
            }
            LOGV("Memory         : %d GB",tt);
        }
        {
            char value[93] = "";
            __system_property_get("ro.build.version.release", value);
            LOGV("Android Version: %s",value);
        }
        {
            char value[93] = "";
            __system_property_get("ro.build.version.sdk", value);
            LOGV("Android API    : %s",value);
        }
#endif
    }

    void Utils::TestLogLevel() {
        LOGV("This is log verbose !")
        LOGD("This is log   debug !")
        LOGI("This is log    info !")
        LOGW("This is log warning !")
        LOGE("This is log   error !")
        LOGF("This is log   fatal !")
    }

}
