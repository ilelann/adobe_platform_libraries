#!/bin/bash
set -e

: "${BUILDDIR:?BUILDDIR path required}"
: "${TOOLSET:?TOOLSET required (xcode or your c++ compiler)}"
: "${USE_SYSTEM_BOOST:?USE_SYSTEM_BOOST required (ON/OFF)}"
: "${USE_STD_THREAD_LOCAL:?USE_STD_THREAD_LOCAL required (ON/OFF)}"

SRC_PATH=$(pwd)

[[ ${USE_SYSTEM_BOOST} = "ON" ]] && BOOST_SUBDIR="SYSBOOT" || BOOST_SUBDIR="LOCBOOST"
[[ ${USE_STD_THREAD_LOCAL} = "ON" ]] && THREADLOCAL_SUBDIR="STDTHREADLOCAL" || THREADLOCAL_SUBDIR="BOOSTTHREADLOCAL"
BUILD_PATH=${BUILDDIR}/${TOOLSET}/${BUILDMODE}/${BOOST_SUBDIR}/${THREADLOCAL_SUBDIR}
mkdir -p ${BUILD_PATH}
pushd ${BUILD_PATH}

if [ "$TOOLSET" == "xcode" ]; then
    cmake -DUSE_STD_THREAD_LOCAL=${USE_STD_THREAD_LOCAL} -DUSE_SYSTEM_BOOST=${USE_SYSTEM_BOOST} -DCMAKE_CXX_COMPILER=${TOOLSET} -DCMAKE_BUILD_TYPE=${BUILDMODE} -G "Xcode" ${SRC_PATH}
    #make -j4
else

    GENERATOR="Unix Makefiles"
    BUILDCMD="make -j4"

    #GENERATOR="Ninja"
    #BUILDCMD="ninja-build"

    cmake -DUSE_STD_THREAD_LOCAL=${USE_STD_THREAD_LOCAL} -DUSE_SYSTEM_BOOST=${USE_SYSTEM_BOOST} -DCMAKE_CXX_COMPILER=${TOOLSET} -DCMAKE_BUILD_TYPE=${BUILDMODE} -G "${GENERATOR}" ${SRC_PATH}
    ${BUILDCMD}
    # "ctest -C ${BUILDMODE}" handles CONFIGURATIONS option of CMake's add_test
    # "make test" only run tests declared without any CONFIGURATIONS flag
    ctest --output-on-failure -C ${BUILDMODE}
fi

popd

