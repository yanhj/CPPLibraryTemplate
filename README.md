# CustomLib

builder Custom library
- 修改 cmakelists.txt 的 LIB_NAME 为你的库名

- 要编译未可执行程序时，将 BUILD_EXEC 设置为 ON
set(BUILD_EXEC ON)

- 要编译为动态库时，将 BUILD_SHARED_LIBS 设置为 ON, 否则为静态库
set(BUILD_SHARED_LIBS OFF)

- 要编译为Framework时，将 BUILD_FRAMEWORK 设置为 ON
set(BUILD_FRAMEWORK OFF)

- 修改log/log.cpp中的 g_logTag 为你的库名
- 修改log/log.cpp中的 g_logLevel 为你的日志级别