cmake_minimum_required(VERSION 3.19)

set(CMAKE_OSX_DEPLOYMENT_TARGET "10.14" CACHE STRING "Minimum OS X deployment version")
set(CMAKE_OSX_ARCHITECTURES x86_64;arm64)
#
# default
#   build static library
# BUILD_SHARED_LIBS
#   build shared library
# BUILD_EXEC
#   build exec
# BUILD_SCRIPT
#   build in script
#
set(LIB_NAME "CustomLib")

project(${LIB_NAME})

set(EXEC_NAME "${LIB_NAME}_exec")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR})
include(ImportAutoGroupFunction)
include(ImportHeadersHelper)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR}/3rdparty/modules/CMakeModule)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR}/3rdparty/CMakeModules)

include(ImportPlatformDefined)
include(builder_local/Config/ProjectVersion-config)

set(BUILD_EXEC OFF)
#BUILD_SHARED_LIBS 通过外部传入
#set(BUILD_SHARED_LIBS OFF)
set(BUILD_FRAMEWORK ON)

# 引入配置
if (DEFINED BUILDER_PROJECT_CONFIG_FILE_PATH)
    include("${BUILDER_PROJECT_CONFIG_FILE_PATH}")
endif()

if (MSVC)
    set(BUILD_FRAMEWORK OFF)
    add_definitions(-DCUSTOM_BUILD)
    add_compile_options(/EHsc)
    # 解决Windows下需要UTF-8带签名编码字符集问题
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/source-charset:utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")
    # release 下生成调试符号
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG:FULL /OPT:REF /OPT:ICF")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG:FULL /OPT:REF /OPT:ICF")
endif()

# enable c++11
set(CMAKE_CXX_STANDARD 11)

# [*] set build flag
if (BUILD_SCRIPT)
    add_definitions(-DCUSTOM_BUILD)
endif ()

# [*] set static flag
if (NOT BUILD_SHARED_LIBS)
    add_definitions(-DCUSTOM_STATIC)
endif (NOT BUILD_SHARED_LIBS)

# add include
include_directories(
        ${CMAKE_CURRENT_LIST_DIR}/src
)

# install header
file(GLOB_RECURSE HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/src/header/*.h
        ${CMAKE_CURRENT_LIST_DIR}/src/header/*.hpp
        )

set(HEADERS_ROOT
        ${CMAKE_CURRENT_LIST_DIR}/src/header/
        )

include_directories(
        ${HEADERS_ROOT}
)

# add sources
file(GLOB_RECURSE SOURCE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/*.h
        ${CMAKE_CURRENT_LIST_DIR}/src/*.hpp
        )

if(NOT MSVC AND NOT BUILD_SCRIPT)
    set(CMAKE_INSTALL_PREFIX "./")
endif()

if (BUILD_EXEC)
    message(STATUS "build exec: ${EXEC_NAME}")
    add_executable(${EXEC_NAME} ${SOURCE_FILES})

    if (CMAKE_PLATFORM_ANDROID)
        target_link_libraries(
                ${EXEC_NAME}
                log
        )
    endif()

elseif(BUILD_SHARED_LIBS)
    message(STATUS "BUILD_SHARED_LIBS: shared -- ${LIB_NAME}")
    if (BUILD_FRAMEWORK)
        add_library(${LIB_NAME} SHARED ${SOURCE_FILES} ${HEADERS})
    else()
        add_library(${LIB_NAME} SHARED ${SOURCE_FILES})
    endif ()

    if (CMAKE_PLATFORM_ANDROID)
        target_link_libraries(
                ${LIB_NAME}
                log
        )
    endif()

else()

    if (BUILD_FRAMEWORK)
        add_library(${LIB_NAME} ${SOURCE_FILES} ${HEADERS})
    else()
        message(STATUS "BUILD_SHARED_LIBS: static -- ${LIB_NAME}")
        add_library(${LIB_NAME} STATIC ${SOURCE_FILES})
    endif ()

endif ()

if (BUILD_EXEC)
    target_link_libraries(${EXEC_NAME} ${dependencies_LIBS} )
else ()
    target_link_libraries(${LIB_NAME} ${dependencies_LIBS} )
endif ()
if (BUILD_EXEC)
    # install exec
    install(TARGETS ${EXEC_NAME}
            DESTINATION "bin"
            )
elseif (BUILD_FRAMEWORK)

    set_target_properties(${LIB_NAME} PROPERTIES
            FRAMEWORK TRUE
            FRAMEWORK_VERSION A
            MACOSX_FRAMEWORK_IDENTIFIER com.meitu.${LIB_NAME}
            # "current version" in semantic format in Mach-O binary file
            VERSION 1.0.0
            # "compatibility version" in semantic format in Mach-O binary file
            SOVERSION 1.0.0
            )

    function_framework_headers("${HEADERS_ROOT}" "${HEADERS}")

    if(BUILD_SHARED_LIBS)
        # install library
        install(TARGETS ${LIB_NAME}
                DESTINATION "${CMAKE_INSTALL_PREFIX}../install/macos/shared/"
        )
    else (BUILD_SHARED_LIBS)
        # install library
        install(TARGETS ${LIB_NAME}
                DESTINATION "${CMAKE_INSTALL_PREFIX}../install/macos/static/"
        )
    endif ()
else()

    #set_target_properties(${LIB_NAME} PROPERTIES
    #    PREFIX "lib"
    #    OUTPUT_NAME "${LIB_NAME}"
    #)

    function_install_headers("../install/windows/include/CustomLib" "${HEADERS_ROOT}" "${HEADERS}")

    if(BUILD_SHARED_LIBS)
        # install library
        install(TARGETS ${LIB_NAME}
                CONFIGURATIONS Release
                DESTINATION "../install/windows/x64/shared/release"
                )

        install(TARGETS ${LIB_NAME}
                CONFIGURATIONS Debug
                DESTINATION "../install/windows/x64/shared/debug"
                )
        #动态库才需要安装pdb文件
        #install pdb file
        install(FILES $<TARGET_PDB_FILE:${PROJECT_NAME}>
                CONFIGURATIONS Release
                DESTINATION "../install/windows/x64/shared/release" OPTIONAL)
        install(FILES $<TARGET_PDB_FILE:${PROJECT_NAME}>
            CONFIGURATIONS Debug
            DESTINATION "../install/windows/x64/shared/debug" OPTIONAL)
    else (BUILD_SHARED_LIBS)
        # install library
        install(TARGETS ${LIB_NAME}
                CONFIGURATIONS Release
                DESTINATION "../install/windows/x64/static/release"
                )

        install(TARGETS ${LIB_NAME}
                CONFIGURATIONS Debug
                DESTINATION "../install/windows/x64/static/debug"
                )
    endif ()
endif()
