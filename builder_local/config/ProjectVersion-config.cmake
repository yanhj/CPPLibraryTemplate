# 用于识别项目版本 json文件

message(STATUS "cmake read builder_local/config/config.json")

# 读取json文件
file(READ "${CMAKE_CURRENT_LIST_DIR}/config.json" project_version_config_json)

# 判断 ${CMAKE_MINIMUM_REQUIRED_VERSION} 是否大于等于 3.19
if(${CMAKE_MINIMUM_REQUIRED_VERSION} VERSION_LESS 3.19.0)
    message(FATAL_ERROR "CMake version must >= 3.19.0 , please update CMake")
endif()


message(STATUS ${project_version_config_json})
# require cmake version >= 3.19
string(JSON V_PROJ_NAME     GET ${project_version_config_json}  project)
string(JSON V_PROJ_VERSION  GET ${project_version_config_json}  version)
string(JSON V_PROJ_TYPE     GET ${project_version_config_json}  type)
string(JSON V_PROJ_BUILD    GET ${project_version_config_json}  build)
string(JSON V_PROJ_SUFFIX   GET ${project_version_config_json}  suffix)
string(JSON V_PROJ_AUTHOR   GET ${project_version_config_json}  author)

message(STATUS "Config V_PROJ_NAME: ${V_PROJ_NAME}")
message(STATUS "Config V_PROJ_VERSION: ${V_PROJ_VERSION}")
message(STATUS "Config V_PROJ_TYPE: ${V_PROJ_TYPE}")
message(STATUS "Config V_PROJ_BUILD: ${V_PROJ_BUILD}")
message(STATUS "Config V_PROJ_SUFFIX: ${V_PROJ_SUFFIX}")
message(STATUS "Config V_PROJ_AUTHOR: ${V_PROJ_AUTHOR}")

set(proj_version_dst_file ${CMAKE_CURRENT_SOURCE_DIR}/src/ProjectVersion.h)
FILE(REMOVE ${proj_version_dst_file})
configure_file(${CMAKE_CURRENT_LIST_DIR}/ProjectVersion.h.in ${proj_version_dst_file})
