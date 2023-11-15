#!/bin/sh
set -e

CURRENT_DIR=$(cd `dirname $0`; pwd)

. ${CURRENT_DIR}/../depends/git.deploy.config.sh
. ${CURRENT_DIR}/../depends/config.sh

pushd ${CURRENT_DIR}
# 检查git仓库中是否存在tag
TEMP_DIR="${CURRENT_DIR}/../build_app/publish/TEMP_CHECK_VERSION"
rm -rf ${TEMP_DIR}
mkdir -p "${TEMP_DIR}"

if [ -n "${BUILDER_GIT_BINARY_BASE_BRANCH}" ]; then
git clone -b ${BUILDER_GIT_BINARY_BASE_BRANCH} ${PUBLISH_GIT_URL} ${TEMP_DIR}
else
    git clone ${PUBLISH_GIT_URL} ${TEMP_DIR}
fi
pushd ${TEMP_DIR}
git tag -l | xargs git tag -d
git fetch origin --prune
RESULT=`git tag -l "${BUILDER_CONFIG_USE_TAG}"`
popd # ${TEMP_DIR}
rm -rf ${TEMP_DIR}
if [[ ${#RESULT} -gt 0  ]]; then
    echo "Version already exist ! (tag: ${BUILDER_CONFIG_USE_TAG})"
    exit -1
fi

popd # ${CURRENT_DIR}
