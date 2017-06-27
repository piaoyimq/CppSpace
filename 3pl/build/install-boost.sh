#!/bin/bash
set -ex

source $WS_ROOT/3pl/build/common.sh $1

check_library_exist $1

BOOST_VERSION="boost_1_60_0.tar.gz"

install()
{
    cd `basename ${BOOST_VERSION} .tar.gz`
    ./bootstrap.sh --prefix=$PACKAGE_PATH
    ./b2 install
    mkdir_compile_time_path
    cp -rf ${PACKAGE_PATH}/include/* $COMPILE_TIME_INCLUDE_PATH
    cp -rf ${PACKAGE_PATH}/lib/* $COMPILE_TIME_LIB_PATH
}

cd $WS_ROOT/3pl/sources/

if [ ! -d `basename ${BOOST_VERSION} .tar.gz` ]
then 
    if [ ! -f $BOOST_VERSION ]
    then
        wget http://cfhcable.dl.sourceforge.net/project/boost/boost/1.60.0/${BOOST_VERSION}
        tar zxf $BOOST_VERSION
    else
        tar zxf $BOOST_VERSION
    fi
fi

install
post_cmake $1

