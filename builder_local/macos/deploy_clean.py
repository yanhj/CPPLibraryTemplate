import sys
import os
import shutil


def find_framework_entries(framework_root):
    ret = []
    with os.scandir(framework_root) as it:
        for entry in it:
            if entry.name.endswith(".framework") and entry.is_dir(follow_symlinks=False):
                framework_path = os.path.join(framework_root, entry.name)
                ret.append(framework_path)
    return ret


def clean_framework(framework_path):
    header_path = os.path.join(framework_path, "Headers")
    if os.path.islink(header_path):
        real_header_path = os.path.realpath(header_path)
        os.unlink(header_path)
        shutil.rmtree(real_header_path)
    elif os.path.isdir(header_path):
        shutil.rmtree(header_path)


def clean_frameworks_header(framework_root):
    abs_path = os.path.abspath(framework_root)
    print("framework_search_dir=" + abs_path)
    framework_list = find_framework_entries(abs_path)
    for framework_path in framework_list:
        clean_framework(framework_path)


search_dir = sys.argv[1]
clean_frameworks_header(search_dir)
