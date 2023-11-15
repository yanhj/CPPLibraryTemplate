
#####################################################################################################################
# 该函数实现对cmake工程中的文件进行自动分组
# function_files_auto_group prefix files
# 参数 prefix 表示文件所在的目录
# 参数 files 为需要分组的文件
#
# 例如:
# set(prefix "/path/to/files")
# set(files ${files} "/path/to/files/file0.cpp")
# set(files ${files} "/path/to/files/group1/file1.cpp")
# set(files ${files} "/path/to/files/group2/file2.cpp")
# 通过调用 (注意需要带上双引号 "")
# function_files_auto_group("${prefix}" "${files}")
# 之后的目录结构为
# .
# ├── file0.cpp
# ├── group1
# │   └── file1.cpp
# └── group2
#     └── file1.cpp
#####################################################################################################################
if (NOT IS_DEFINED_IMPORT_AUTO_GROUP_FUNCTION)
    set(IS_DEFINED_IMPORT_AUTO_GROUP_FUNCTION TRUE)

    function(function_files_auto_group prefix files)
        # 设置当前目录
        SET(cur_dir ${CMAKE_CURRENT_SOURCE_DIR})
        # 遍历循环每一个文件
        foreach(filename ${files})
            # 截取文件在工程中的目录
            STRING(REGEX REPLACE "${cur_dir}" "" fpath ${filename})
            # 路径中最后一个 / 的位置（从后往前）
            STRING(FIND ${fpath} "/" lastidx REVERSE)
            # 文件所在的目录层级路径
            STRING(SUBSTRING ${fpath} 0 ${lastidx} fakegroupname)
            # 目录层级路径是否为空
            STRING(COMPARE EQUAL "${fakegroupname}" "" bequal)
            # 如果不为空
            IF(NOT ${bequal} EQUAL 1)
                # 替换斜杠
                STRING(REPLACE "/" "\\" groupname ${fakegroupname})

                # 计算组名长度
                string(LENGTH ${groupname} igrouplength)

                # 最终的分组名
                set(groupname2 ${groupname})

                # 如果组名长度大于0
                if(${igrouplength} GREATER 0)
                    # 去除开头的斜杠
                    STRING(SUBSTRING ${groupname} 1 ${igrouplength}-1 groupname2)
                ELSE(${igrouplength} GREATER 0)
                    # 否则设置为根目录
                    SET(groupname2 "\\")
                ENDIF(${igrouplength} GREATER 0)
            ELSE(NOT ${bequal} EQUAL 1)
                # 设置为根目录工程
                SET(groupname2 "\\")
            ENDIF(NOT ${bequal} EQUAL 1)
            SOURCE_GROUP(${groupname2} FILES ${filename})
        endforeach(filename ${files})
    endfunction(function_files_auto_group prefix files)
    
    # 是否定义函数 function_files_auto_group
    set(IS_DEFINED_FUNCTION_FILES_AUTO_GROUP TRUE)

endif ()
