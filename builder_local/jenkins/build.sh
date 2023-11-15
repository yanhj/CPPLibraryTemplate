#!/bin/bash
set -e

CURRENT_DIR=$(cd `dirname $0`; pwd)
. ${CURRENT_DIR}/../depends/config.sh
. ${CURRENT_DIR}/../depends/default.config.sh

pushd ${CURRENT_DIR}

DEFAULT_QT_5_HOME=“”
QT_PATH=""
QT_5_PATH=""
QT_5_BOOL=true

# Qt5 路径搜索
if [ -d "$DEFAULT_QT_5_HOME" ]; then
	QT_5_PATH=$DEFAULT_QT_5_HOME
elif [ $QT_5_15_13_HOME ]; then
  QT_5_PATH=$QT_5_15_13_HOME
else
  echo "Qt5 config failed,no set QT_5_HOME..."
  QT_5_BOOL=false
fi

if [[ $QT_5_BOOL = true ]]; then
  echo "Config Qt5 success $QT_5_PATH"
fi

# Qt5 识别异常 退出
if [ $QT_5_BOOL = false ]; then
  echo "Qt5 config failed , EXIT 1 "
  exit 1
fi

echo "buildType=${BUILDER_BUILD_TYPE}"
PACKET_TIME=$(date "+%Y/%m/%d-%H:%M:%S")


rm -rf ${BUILD_ROOT}

echo "start build...."
QT_PATH=$QT_5_PATH
BUILD_ROOT_DIR=${BUILD_ROOT}/macOS
BUILD_SHADER="${CURRENT_DIR}/../macos/macos.build.sh"
CONFIG_PATH="${CURRENT_DIR}/../config/option-config-cmake/app.option.config.cmake"
echo "BUILD_ROOT_DIR : ${BUILD_ROOT_DIR}"

echo "execute ${BUILD_SHADER} ${BUILD_ROOT_DIR} ${CONFIG_PATH}"
${BUILD_SHADER} ${BUILD_ROOT_DIR} ${CONFIG_PATH} ${QT_PATH}

popd # ${CURRENT_DIR}
