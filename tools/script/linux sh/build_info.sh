# /bin/bash
top_dir=`pwd`
buildFilePath=./app_build_info.h

function get_git_version() {
    GIT_BRANCH=$(git rev-parse --abbrev-ref HEAD)
    GIT_TAG=$(git describe --tags --abbrev=0)
    GIT_HASH=$(git log --pretty=format:"%h" -1)
    GIT_DATETIME=$(git show -s --format=%cd --date=short | sed 's/-//g')

	if [ ${GIT_TAG} ]; then
		GIT_COUNT=$(git rev-list --count ${GIT_TAG}..HEAD)
        GIT_VERSION=$GIT_BRANCH-$GIT_TAG.$GIT_COUNT-$GIT_HASH-$GIT_DATETIME
    else
		GIT_VERSION=$GIT_BRANCH-$GIT_COUNT-$GIT_HASH-$GIT_DATETIME
	fi

    echo $GIT_VERSION
}

GIT_VERSION=$(get_git_version)
DATETIME=$(git show -s --format=%cd --date=short | sed 's/-//g')

GIT_BRANCH=`git branch 2>/dev/null | grep "*" | awk '{print $2}'`
GIT_TAG=`git describe --tags --always --dirty=-X 2>/dev/null | sed 's/\//-/g'`

# 这里是不是还缺一个编译时间,但是linux时间不一定准确
echo "#ifndef __ZAYK_BUILD_INFO_H__" > $buildFilePath
echo "#define __ZAYK_BUILD_INFO_H__" >> $buildFilePath
echo "    #define GIT_BRANCH \"$GIT_BRANCH\"" >> $buildFilePath
echo "    #define GIT_TAG \"$GIT_TAG\"" >> $buildFilePath
echo "    #define GIT_HASH \"$GIT_HASH\"" >> $buildFilePath
echo "    #define GIT_DATETIME \"$GIT_DATETIME\"" >> $buildFilePath
echo "    #define GIT_COMMIT_HASH \"$GIT_VERSION\"" >> $buildFilePath
echo "#endif" >> $buildFilePath