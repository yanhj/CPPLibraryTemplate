
#####################################################################################################################
# function_framework_headers headers_root headers
# 参数 headers_root 表示头文件所在根目录
# 参数 headers      表示需要安装的头文件
#
# 该函数实现对Apple平台framework下头文件保持目录结构的实现
#
# 例如:
# set(headers_root "/path/to/headers/example")
# set(headers ${headers} "/path/to/headers/example/example.h")
# set(headers ${headers} "/path/to/headers/example/a/a.h")
# set(headers ${headers} "/path/to/headers/example/b/b.h")
# 通过调用 (注意需要带上双引号 "")
# function_framework_headers("${headers_root}" "${headers}")
# 在framework中的头文件目录结构即可保持
#
#
# function_install_headers prefix headers_root headers
# 参数 prefix       表示头文件安装前缀
# 参数 headers_root 表示头文件所在根目录
# 参数 headers      表示需要安装的头文件
#
# 该函数实现编译头文件生成保持目录结构
#
# 例如:
# set(prefix "include/example")
# set(headers_root "/path/to/headers/example")
# set(headers ${headers} "/path/to/headers/example/example.h")
# set(headers ${headers} "/path/to/headers/example/a/a.h")
# set(headers ${headers} "/path/to/headers/example/b/b.h")
# 通过调用 (注意需要带上双引号 "")
# function_install_headers("${prefix}" "${headers_root}" "${headers}")
# 编译安装后的头文件结构即可保持
#####################################################################################################################
if (NOT IS_DEFINED_IMPORT_HEADERS_HELPER)
    set(IS_DEFINED_IMPORT_HEADERS_HELPER TRUE)

    function(function_framework_headers headers_root headers)

        foreach(h_file ${headers})
            string(REPLACE "${headers_root}" "" h_file_2 "${h_file}")
            string(FIND "${h_file_2}" "/" pos REVERSE)
            if ( ${pos} LESS 0 )
                set_property(SOURCE "${h_file}" PROPERTY MACOSX_PACKAGE_LOCATION Headers)
            else ()
                string(SUBSTRING "${h_file_2}" 0 ${pos} dir)
                set_property(SOURCE "${h_file}" PROPERTY MACOSX_PACKAGE_LOCATION Headers/${dir})
            endif ()
        endforeach(h_file)

    endfunction(function_framework_headers headers_root headers)

    # 是否定义函数 function_framework_headers
    set(IS_DEFINED_FUNCTION_FRAMEWORK_HEADERS TRUE)

    function(function_install_headers prefix headers_root headers)

        foreach(h_file ${headers})
            string(REPLACE "${headers_root}" "" h_file_2 "${h_file}")
            string(FIND "${h_file_2}" "/" pos REVERSE)
            if ( ${pos} LESS 0 )
                install(FILES "${h_file}" DESTINATION "${prefix}")
            else ()
                string(SUBSTRING "${h_file_2}" 0 ${pos} dir)
                install(FILES "${h_file}" DESTINATION "${prefix}/${dir}")
            endif ()
        endforeach(h_file)

    endfunction(function_install_headers prefix headers_root headers)

    # 是否定义函数 function_install_headers
    set(IS_DEFINED_FUNCTION_INSTALL_HEADERS TRUE)

endif ()
