# Path: build-script/py/clean_redundant_content.py

import os
import sys
import json
import shutil


def remove_path(path):
    # 判断路径是否存在
    if os.path.exists(path):
        print("remove path --- ", path)
        # 判断路径是否为文件夹
        if os.path.isdir(path):
            # 移除文件夹
            shutil.rmtree(path)
        else:
            # 移除文件
            os.remove(path)

# 识别当前平台
def get_platform():
    platform = sys.platform
    if platform == "darwin":
        return "macos"
    elif platform == "win32":
        return "windows"
    else:
        return None


def read_config(json_path,platform,append_prefix_path):
    path_list = []
    with open(json_path, "r") as f:
        clean_config = json.load(f)

    config_dict = clean_config[platform]
    for path in config_dict:
        path = os.path.join(append_prefix_path, path)
        path_list.append(path)

    return path_list


def main(argv):
    # 获取当前平台
    platform = get_platform()
    if platform is None:
        print("Not support this platform")
        return
    # 获取当前python脚本的路径
    current_path = os.path.dirname(os.path.realpath(__file__))

    # 获取当前app的路径
    app_path = argv[1]
    # 获取当前clean-config.json的路径
    clean_config_path = os.path.join(current_path, "clean-config.json")
    # 获取当前平台所需移除的路径
    path_list = read_config(clean_config_path, platform, app_path)
    # 移除路径
    for path in path_list:
        remove_path(path)



if __name__ == "__main__":
    main(sys.argv)