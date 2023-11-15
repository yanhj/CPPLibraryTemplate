#!/bin/bash
set -e

CURRENT_DIR=$(cd `dirname $0`; pwd)
pushd ${CURRENT_DIR}

rm -rf "${CURRENT_DIR}/../build_app"

git checkout .
git clean -df
git reset --hard
git submodule update --init --recursive

. ${CURRENT_DIR}/../../third_party/update_gitem_fast.sh

popd # ${CURRENT_DIR}
