# -*- coding=utf-8 -*-
import argparse
import os
import platform
import subprocess
import logging
import sys
import socket
import getpass

logging.getLogger().setLevel(logging.INFO)


class Env:
    """
    运行环境检测
    """
    def __init__(self):
        self.__host_info = ""
        self.__host_platform = ""

        self.__detect_host()
        self.__generate_host_info()

    def host_info(self) -> str:
        return self.__host_info

    def print(self):
        logging.info(self.host_info())
        logging.info('++++++++++ environment ++++++++++')
        for key in os.environ.keys():
            logging.info(key + ':' + os.environ.get(key))
        logging.info('---------- environment ----------')

    def __detect_host(self):
        if platform.system() == 'Darwin':
            self.__host_platform = "macOS"
            return
        elif platform.system() == 'Windows':
            self.__host_platform = "Windows"
            return
        elif platform.system() == 'Linux':
            if os.path.isfile('/etc/os-release'):
                with open('/etc/os-release', mode='r', encoding='utf-8') as fd:
                    for line in fd:
                        if 'Ubuntu' in line:
                            self.__host_platform = "Ubuntu"
                            return
                        if 'CentOS' in line:
                            self.__host_platform = "CentOS"
                            return

        raise Exception(str('[{}] unknown system !').format(self.__class__.__name__))

    def __generate_host_info(self):
        self.__host_info = '['
        self.__host_info += getpass.getuser()
        self.__host_info += ', '
        self.__host_info += socket.gethostname()
        self.__host_info += ', '
        self.__host_info += platform.platform()
        self.__host_info += ']'


class LocalHelper:
    @staticmethod
    def adqm(value: str) -> str:
        """
        append double quotation marks
        :param value:   value
        :return:        "value"
        """
        return '\"' + value + '\"'

    @staticmethod
    def run_command(cmd: str, cwd: str):
        """
        执行命令行代码

        :param cmd:     命令行代码
        :param cwd:     代码执行目录
        :return:        None
        """
        return_code = subprocess.call(cmd, shell=True, cwd=cwd)
        log = str('[subprocess={}] cmd=({}) cwd=({})').format(return_code, cmd, cwd)
        if return_code != 0:
            raise Exception(log)
        else:
            logging.info(log)

    @staticmethod
    def python_command() -> str:
        command = 'python3'
        if 'Windows' in platform.system():
            command = 'python'

        return command


class Local:
    """
    用户本地自定义类

    按需修改此类即可
    """
    def __init__(self):
        self.working_directory = os.path.dirname(os.path.realpath(__file__))

    def check(self):
        logging.info("local::check")
        Env().print()
        command = './jenkins/check-version.sh'
        LocalHelper.run_command(command, self.working_directory)

    def init(self):
        logging.info("local::init")
        Env().print()
        command = './jenkins/clean.sh'

        if platform.system() == 'Windows':
            command = ".\\jenkins\\clean.bat"

        LocalHelper.run_command(command, self.working_directory)

    def build(self, platform_str):
        logging.info("local::build " + platform_str)
        Env().print()
        
        if platform_str == 'windows':
            command = ".\\jenkins\\build.bat"
            LocalHelper.run_command(command, self.working_directory)
        elif platform_str == 'macos':
            command = "./jenkins/build.sh"
            LocalHelper.run_command(command, self.working_directory)
        elif platform_str == 'ubuntu':
            command = "./jenkins/build.linux.sh"
            LocalHelper.run_command(command, self.working_directory)
            command = "./jenkins/build.ubuntu.sh"
            LocalHelper.run_command(command, self.working_directory)
        elif platform_str == 'centos':
            command = "./jenkins/build.centos.sh"
            LocalHelper.run_command(command, self.working_directory)


    def cache(self):
        logging.info("local::cache")
        Env().print()

        if platform.system() == 'Windows':
            command = ".\\Windows\\publish.bat"
            LocalHelper.run_command(command, self.working_directory)

    def publish(self):
        logging.info(r'Running publish {}:{}'.format(sys.platform, platform.system()))
        if platform.system() == 'Darwin':
            command = './macos/publish.sh'
            LocalHelper.run_command(command, self.working_directory)


class EnterPoint:
    """
    入口点
    """
    __local = None

    @staticmethod
    def local() -> Local:
        if EnterPoint.__local is None:
            EnterPoint.__local = Local()
        return EnterPoint.__local

    @staticmethod
    def check(args):
        EnterPoint.local().check()

    @staticmethod
    def init(args):
        EnterPoint.local().init()

    @staticmethod
    def build(args):
        EnterPoint.local().build(args.platform)

    @staticmethod
    def cache(args):
        EnterPoint.local().cache()

    @staticmethod
    def publish(args):
        EnterPoint.local().publish()


if __name__ == '__main__':
    argc = len(sys.argv)

    # 参数解析
    parser = argparse.ArgumentParser(prog='local')

    # 子命令
    subparsers = parser.add_subparsers(help='operate function')

    # 添加子命令 check
    parser_check = subparsers.add_parser('check', help='check current build validity')
    parser_check.set_defaults(func=EnterPoint.check)

    # 添加子命令 init
    parser_init = subparsers.add_parser('init', help='init and clean cache')
    parser_init.set_defaults(func=EnterPoint.init)

    # 添加子命令 build
    parser_build = subparsers.add_parser('build', help='build target platform')
    parser_build.set_defaults(func=EnterPoint.build)
    parser_build.add_argument('-p', '--platform', choices=['windows', 'macos', 'centos', 'ubuntu', 'ios', 'android'],
                              type=str, help='target platform', required=True)

    # 添加子命令 cache
    parser_cache = subparsers.add_parser('cache', help='cache products')
    parser_cache.set_defaults(func=EnterPoint.cache)

    # 添加子命令 publish
    parser_publish = subparsers.add_parser('publish', help='publish version')
    parser_publish.set_defaults(func=EnterPoint.publish)

    # 解析
    args_parser = parser.parse_args()

    # 日志等级
    if hasattr(args_parser, 'verbose') and args_parser.verbose:
        logging.getLogger().setLevel(logging.DEBUG)
        EnterPoint.local().verbose = True

    # check or init or build or cache or publish
    if 'func' in args_parser:
        args_parser.func(args_parser)
