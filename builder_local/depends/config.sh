#!/bin/bash
set -e

if [[ ${BUILDER_CONFIG_PATH} == "" ]]; then
    CONFIG_SH_CURRENT_DIR=$(cd `dirname $0`; pwd)
    BUILDER_CONFIG_PATH="${CONFIG_SH_CURRENT_DIR}/../config/config.json"
fi

# 项目名
CONFIG_PROJECT=`cat ${BUILDER_CONFIG_PATH} | python3 -c "import json; import sys; obj=json.load(sys.stdin); print (json.dumps(obj['project']))"`
BUILDER_CONFIG_PROJECT=${CONFIG_PROJECT//\"}
# 版本名
CONFIG_VERSION=`cat ${BUILDER_CONFIG_PATH} | python3 -c "import json; import sys; obj=json.load(sys.stdin); print (json.dumps(obj['version']))"`
BUILDER_CONFIG_VERSION=${CONFIG_VERSION//\"}
# 构建类型
CONFIG_BUILD_TYPE=`cat ${BUILDER_CONFIG_PATH} | python3 -c "import json; import sys; obj=json.load(sys.stdin); print (json.dumps(obj['type']))"`
BUILDER_BUILD_TYPE=${CONFIG_BUILD_TYPE//\"}
# 构建次数
CONFIG_BUILD=`cat ${BUILDER_CONFIG_PATH} | python3 -c "import json; import sys; obj=json.load(sys.stdin); print (json.dumps(obj['build']))"`
BUILDER_CONFIG_BUILD=${CONFIG_BUILD//\"}
# 版本后缀
CONFIG_SUFFIX=`cat ${BUILDER_CONFIG_PATH} | python3 -c "import json; import sys; obj=json.load(sys.stdin); print (json.dumps(obj['suffix']))"`
BUILDER_CONFIG_SUFFIX=${CONFIG_SUFFIX//\"}
# 作者
CONFIG_BUILDER_AUTHOR=`cat ${BUILDER_CONFIG_PATH} | python3 -c "import json; import sys; obj=json.load(sys.stdin); print (json.dumps(obj['author']))"`
BUILDER_CONFIG_AUTHOR=${CONFIG_BUILDER_AUTHOR//\"}


pyscript="${CONFIG_SH_CURRENT_DIR}/../depends/pack_utils.py"
BUILDER_AUTHOR=`python3 ${pyscript} author ${BUILDER_CONFIG_PATH}`

echo “BUILDER_AUTHOR: ” ${BUILDER_AUTHOR}



BUILDER_CONFIG_PACK_VERSION_STR="${BUILDER_CONFIG_VERSION}-${BUILDER_BUILD_TYPE}-${BUILDER_CONFIG_BUILD}-${BUILDER_CONFIG_SUFFIX}"
BUILDER_CONFIG_USE_TAG="${BUILDER_CONFIG_VERSION}-${BUILDER_BUILD_TYPE}-${BUILDER_CONFIG_BUILD}-${BUILDER_CONFIG_SUFFIX}"
BUILDER_GIT_BINARY_BASE_BRANCH=""

# 移除尾部的破折号
BUILDER_CONFIG_PACK_VERSION_STR="${BUILDER_CONFIG_PACK_VERSION_STR%%-}"
BUILDER_CONFIG_USE_TAG="${BUILDER_CONFIG_USE_TAG%%-}"
echo "BUILDER_CONFIG_PACK_VERSION_STR: ${BUILDER_CONFIG_PACK_VERSION_STR}"
echo "BUILDER_CONFIG_USE_TAG: ${BUILDER_CONFIG_USE_TAG}"
