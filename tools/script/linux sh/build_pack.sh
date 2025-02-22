#!/bin/bash
# 常见的设置，增强了脚本的健壮性
set -Eeuo pipefail

# set -x

PACK_TOOL=pack.sh

# 当前脚本的绝对路径
SCRIPT_DIR=$(dirname $(readlink -f "$0"))
# 获取所有以bin结尾的文件 并且存储到数组FILE_NAME中
FILE_NAME=($SCRIPT_DIR/out/*_H.bin)
# 提取当前脚本所在的目录名称
DIR_NAME=$(basename $SCRIPT_DIR)

# 判断是否生成bin成功
if [ ${#FILE_NAME[@]} -eq 0 ]; then
    echo "没有找到匹配的 .bin 文件！"
    exit 1
fi

# 颜色打印
red(){
    echo -e "\033[31m\033[01m$@\033[0m"
}
green(){
    echo -e "\033[32m\033[01m$@\033[0m"
}
yellow(){
    echo -e "\033[33m\033[01m$@\033[0m"
}
blue(){
    echo -e "\033[34m\033[01m$@\033[0m"
}

# 获取git属性
function get_git_version() {
    GIT_BRANCH=$(git rev-parse --abbrev-ref HEAD) # 获取当前分支
    GIT_TAG=$(git describe --tags --abbrev=0) # 获取tag
    GIT_HASH=$(git log --pretty=format:"%h" -1) # 获取hash
    GIT_DATETIME=$(git show -s --format=%cd --date=short | sed 's/-//g') # 获取提交日期

    if [ ${GIT_TAG} ]; then
        GIT_COUNT=$(git rev-list --count ${GIT_TAG}..HEAD) # 获取提交次数
        GIT_VERSION=$GIT_BRANCH-$GIT_TAG.$GIT_COUNT-$GIT_HASH-$GIT_DATETIME
    else
        GIT_VERSION=$GIT_BRANCH-$GIT_COUNT-$GIT_HASH-$GIT_DATETIME
    fi

    echo $GIT_VERSION
}

GIT_VERSION=$(get_git_version) # 其实就是内容拼接了 branch-name-tag.commit-count-commit-hash-date

# 包含git信息的名字
OUT_NAME=$(basename $FILE_NAME .bin)-$GIT_VERSION
TARGET_NAME=$(basename $FILE_NAME .bin)-$GIT_VERSION.tar.gz

green 输出文件名: $TARGET_NAME

# 清理和准备目录
####################################################################
cd $SCRIPT_DIR/..

[ -d "$OUT_NAME" ] && rm -rf ./$OUT_NAME
[ -f "$TARGET_NAME.tar.gz" ] && rm -rf ./$TARGET_NAME.tar.gz

# 创建目录
mkdir ./$OUT_NAME
mkdir ./$OUT_NAME/out

cp -rf $SCRIPT_DIR/out/*H.bin               ./$OUT_NAME/out/
cp -rf $SCRIPT_DIR/tools                    ./$OUT_NAME/
cp -rf $SCRIPT_DIR/download.sh              ./$OUT_NAME/
cp -rf $SCRIPT_DIR/dev_note/download/*      ./$OUT_NAME/
####################################################################

red tar $OUT_NAME

tar -zcf $TARGET_NAME ./$OUT_NAME
