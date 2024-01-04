#!/bin/bash
set -e

echo "Runing $0 ......"

BUILD_ROOT=$(cd "$(dirname "$0")";pwd)
echo "BUILD_ROOT : ${BUILD_ROOT}"

cd $BUILD_ROOT
BUILD_OUTPUT=${BUILD_ROOT}/../build_app/install/macos/

if [ ! -d "$BUILD_OUTPUT" ]
then
    # Remove local cache.
    echo "build output dir : $BUILD_OUTPUT is null."
    exit 1
fi

COMMIT_HASH=`git rev-parse HEAD`

# Get git publish repository url
. ${BUILD_ROOT}/../depends/git.deploy.config.sh
. ${BUILD_ROOT}/../depends/config.sh

PUBLISH_GIT="${PUBLISH_GIT_URL}"
PUBLISH_NAME="${BUILD_ROOT}/../build_app/publish/sdk_binary"

echo "PUBLISH_GIT : ${PUBLISH_GIT}"

if [ -z ${BRANCH_NAME} ]
then
    # Get branch name from git command
    BRANCH_NAME=$(git branch | sed -n '/*[ ]/s///p')
    echo "Found current branch name ${BRANCH_NAME}"
fi

branch="${BRANCH_NAME}"
headMessage=`git log --abbrev-commit --pretty=oneline -n1`

# Get library version info
#. ./compile-tools/tools/get-version-info.sh
#echo "Version Info: ${PROJECT_VERSION}"
if [ -d "${PUBLISH_NAME}" ]
then
    # Remove local cache.
    echo "local publish library ${PUBLISH_NAME} exist."
    cd ${PUBLISH_NAME}
    git reset --hard
    git clean -xdf
else
    git clone $PUBLISH_GIT ${PUBLISH_NAME}
fi
    cd ${PUBLISH_NAME}
    if [ -d "macos" ]; then
        rm -rf macos
    fi

set +e
isFindBranch=`git rev-parse --verify "origin/${BRANCH_NAME}"`
set -e
if [ -z ${isFindBranch} ]; then
    echo "no find branch ${BRANCH_NAME}"
    set +e
    isFindLocalBranch=`git rev-parse --verify "${BRANCH_NAME}"`
    set -e
    if [ ! -z ${isFindLocalBranch} ]; then
        echo "found loacl branch ${BRANCH_NAME}, delete it"
        git reset --hard
        git clean -xdf
        git checkout main
        git branch -D ${BRANCH_NAME}
    fi
    git checkout -b ${BRANCH_NAME}
else
    echo "found branch ${BRANCH_NAME}"
    git fetch
    git checkout ${BRANCH_NAME}
    git reset --hard origin/${BRANCH_NAME}
fi
                
# Install git lfs support
git lfs install
git lfs track "*.a"
git lfs track "*.dylib"
git lfs track "*.tar.gz"

rm -rf ./macos
echo "cp file..."
# -R : copy all files and directories recursively, keep the file link property
# -f : remove existing destination files
cp -Rf "${BUILD_OUTPUT}"/ ./macos

# 获取tag
tag_name=${BUILDER_CONFIG_USE_TAG}
echo "tag_name: ${tag_name}"

# 切换到publish仓库目录
echo "cd ${PUBLISH_NAME}"
cd ${PUBLISH_NAME}

echo "git add..."
# Push library to server.
git add .
git commit -m "[apollo]: macos-pack ${tag_name}"
git push -u origin ${branch}

echo "pushed..."

if [ -n ${tag_name} ]; then
  #切换到publish仓库目录
  cd ${PUBLISH_NAME}

  #先判断tag是否存在，存在则删除
  # 检查标签是否存在
  if git rev-parse --verify --quiet "refs/tags/${tag_name}"; then
    echo "Git command returned success (tag exists)"
    # 如果存在，则删除标签
    git tag -d "${tag_name}"
    # 同步删除操作到远程仓库（如果需要的话）
    git push --delete origin "${tag_name}"
    echo "Tag ${tag_name} deleted."
  else
    echo "Git command returned failure (tag does not exist)"
    echo "Tag ${tag_name} does not exist."
  fi

  git tag -a "${tag_name}" -m "add tag ${tag_name}"
  git push origin ${tag_name}
fi

# clean workspace
echo "remove repo..."

rm -rf "${PUBLISH_NAME}"

echo "Done $0......"
exit 0