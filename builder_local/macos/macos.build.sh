#!/bin/bash
set -e

# 路径获取
CURRENT_DIR=$(cd `dirname $0`; pwd)
. ${CURRENT_DIR}/../depends/default.config.sh

BUILD_ROOT_DIR=$1
CONFIG_PATH=$2
QT_PATH=$3
INSTALL_PREFIX="./"

if [ -z ${BUILD_ROOT_DIR} ]; then
	BUILD_ROOT_DIR=${DEFAULT_BUILD_ROOT_DIR}
fi

if [ -z ${BUILD_ROOT_DIR} ]; then
	echo "error: invalid BUILD_ROOT_DIR"
	exit
fi

rm -rf ${BUILD_ROOT_DIR}
mkdir -p ${BUILD_ROOT_DIR}
pushd ${BUILD_ROOT_DIR}

if [ "${QT_PATH}" != "" ]; then
	echo "use external definition QT_PATH = ${QT_PATH}"
	QT_HOME_OPTION="-DCMAKE_QT_HOME_DEBUG=ON -DCMAKE_QT_HOME_PATH=${QT_PATH}"
fi

#build shared framework
cmake \
-G "Unix Makefiles" \
-DCMAKE_BUILD_TYPE="Release" \
-DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX} \
-DBUILD_SHARED_LIBS="ON" \
-DBUILDER_PROJECT_CONFIG_FILE_PATH="${CONFIG_PATH}"  \
${QT_HOME_OPTION} \
${PROJET_ROOT}

make all -j8
make install

#build static framework
cmake \
-G "Unix Makefiles" \
-DCMAKE_BUILD_TYPE="Release" \
-DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX} \
-DBUILD_SHARED_LIBS="OFF" \
-DBUILDER_PROJECT_CONFIG_FILE_PATH="${CONFIG_PATH}"  \
${QT_HOME_OPTION} \
${PROJET_ROOT}

make all -j8
make install
popd # ${BUILD_ROOT_DIR}

echo ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> BUILD SUCCESS"
