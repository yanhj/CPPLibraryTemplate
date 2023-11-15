#!/bin/sh
set -e

CURRENT_DIR=$(cd `dirname $0`; pwd)
. ${CURRENT_DIR}/../depends/git.deploy.config.sh
. ${CURRENT_DIR}/../depends/config.sh

pushd ${CURRENT_DIR}
    # 发布 git
    TEMP_DIR="${CURRENT_DIR}/TEMP_PUBLISH"
    #TEMP_NAME="TEMP"
    rm -rf ${TEMP_DIR}
    #mkdir ${TEMP_DIR}
    git clone -b ${BUILDER_GIT_BINARY_BASE_BRANCH} ${BUILDER_GIT_BINARY_URL} ${TEMP_DIR}
    pushd ${TEMP_DIR}
    git fetch origin --prune
    # 检测远程分支是否存在
    TEMP_GIT_BRANCH_R=$(git branch -r)
    if [[ "${TEMP_GIT_BRANCH_R}" =~ "origin/${BUILDER_CONFIG_USE_BRANCH}"  ]]; then
        git checkout -b ${BUILDER_CONFIG_USE_BRANCH} origin/${BUILDER_CONFIG_USE_BRANCH}
    else
        exit -1
    fi
    git branch --set-upstream-to=origin/${BUILDER_CONFIG_USE_BRANCH} ${BUILDER_CONFIG_USE_BRANCH}

    # tag
    git tag "${BUILDER_CONFIG_USE_TAG}"
    git push origin "${BUILDER_CONFIG_USE_TAG}"
    git push origin --delete ${BUILDER_CONFIG_USE_BRANCH}

    popd # ${TEMP_DIR}
    # 上传完就删除
    rm -rf ${TEMP_DIR}
popd # ${CURRENT_DIR}
