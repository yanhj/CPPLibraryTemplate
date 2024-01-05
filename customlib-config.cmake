# libCustomLib 引用规则
# 包含以下可用变量
# libCustomLib_INCLUDE_DIRS    头文件目录
# libCustomLib_LIBS            LinkTarget

set(_ROOT "${CMAKE_CURRENT_LIST_DIR}/")
if(MSVC)
    # windows
    set(libCustomLib_ROOT "${_ROOT}/windows/")
    set(libCustomLib_INCLUDE_DIRS "${libCustomLib_ROOT}/include" )

    set(libCustomLib_LIB_ROOT "${libCustomLib_ROOT}")

    if("${CMAKE_SIZEOF_VOID_P}" EQUAL "4")
        set(libCustomLib_LIB_ROOT "${libCustomLib_LIB_ROOT}/x86")
    else("${CMAKE_SIZEOF_VOID_P}" EQUAL "4")
        set(libCustomLib_LIB_ROOT "${libCustomLib_LIB_ROOT}/x64")
    endif("${CMAKE_SIZEOF_VOID_P}" EQUAL "4")

    set(Lib_Use_Static false)
    if (${Lib_Use_Static})
        add_library(libCustomLib STATIC IMPORTED)
        set(libCustomLib_LIB_ROOT ${libCustomLib_LIB_ROOT}/static)
    else (${Lib_Use_Static})
        add_library(libCustomLib SHARED IMPORTED)
        set(libCustomLib_LIB_ROOT ${libCustomLib_LIB_ROOT}/shared)
    endif ()

    if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        set(libCustomLib_LIB_ROOT "${libCustomLib_LIB_ROOT}/debug")
    else("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        set(libCustomLib_LIB_ROOT "${libCustomLib_LIB_ROOT}/release")
    endif("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")

    if (${Lib_Use_Static})
        set_target_properties(libCustomLib PROPERTIES IMPORTED_LOCATION "${libCustomLib_LIB_ROOT}/CustomLib.lib")
    else (${Lib_Use_Static})
        set_target_properties(libCustomLib PROPERTIES IMPORTED_IMPLIB "${libCustomLib_LIB_ROOT}/CustomLib.lib")
        set(libCustomLib_DYLIBS "${libCustomLib_LIB_ROOT}/CustomLib.dll")
    endif ()
    set(libCustomLib_LIBS libCustomLib)
    message(STATUS "libCustomLib_LIBS: ${libCustomLib}")
else()
    if(ANDROID)
    elseif(APPLE)
        if(IOS)
            # ios
        else()
            # macos
            set(libCustomLib_ROOT "${_ROOT}/macos")
            set(CMAKE_SHARED_LIBRARY_RUNTIME_C_FLAG "-Wl,-rpath,")
            set(libCustomLib_INCLUDE_DIRS "${libCustomLib_ROOT}/include" )
        endif()
        set(Lib_Use_Static false)
        if (${Lib_Use_Static})
            add_library(libCustomLib STATIC IMPORTED)
            set(libCustomLib_ROOT ${libCustomLib_ROOT}/static)
        else (${Lib_Use_Static})
            add_library(libCustomLib SHARED IMPORTED)
            set(libCustomLib_ROOT ${libCustomLib_ROOT}/shared)
        endif ()
        message(STATUS "libCustomLib_ROOT: ${libCustomLib_ROOT}")
        set_target_properties(libCustomLib PROPERTIES IMPORTED_LOCATION "${libCustomLib_ROOT}/CustomLib.framework/CustomLib")

        set(libCustomLib_INCLUDE_DIRS "${libCustomLib_ROOT}/CustomLib.framework")
        set(libCustomLib_FRAMEWORKS "${libCustomLib_ROOT}/CustomLib.framework")
    else()
        # linux
        message(FATAL_ERROR "FATAL: CustomLib not support linux .")
    endif()

    set(libCustomLib_LIBS libCustomLib)
endif()

message(STATUS "libCustomLib_INCLUDE_DIRS=${libCustomLib_INCLUDE_DIRS}")

