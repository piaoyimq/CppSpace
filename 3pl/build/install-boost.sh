#!/bin/bash
set -ex

source $WS_ROOT/3pl/build/common.sh $1

check_library_exist $1

BOOST_VERSION="boost_1_60_0.tar.gz"

install()
{
    cd `basename ${BOOST_VERSION} .tar.gz`
    ./bootstrap.sh
    ./b2 install --prefix=$PACKAGE_PATH/usr/local --build-dir=$OUTPUT_PATH #build the entire boost libraries

#     ./b2 install --debug-configuration -j32  --build-dir=$OUTPUT_PATH --prefix=$PACKAGE_PATH/usr/local --layout=system --with-system --with-filesystem --with-regex --with-timer --with-chrono toolset=gcc link=static 
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
post_handle 

