#!/bin/bash
set -ex

source $WS_ROOT/3pl/build/common.sh $1

check_library_exist $1

POCO_VERSION="poco-1.9.0-all.tar.gz"

install()
{
    cd `basename ${POCO_VERSION} .tar.gz`
    ./bootstrap.sh
    ./b2 install --prefix=$PACKAGE_PATH/usr/local --build-dir=$OUTPUT_PATH #build the entire boost libraries

#     ./b2 install --debug-configuration -j32  --build-dir=$OUTPUT_PATH --prefix=$PACKAGE_PATH/usr/local --layout=system --with-system --with-filesystem --with-regex --with-timer --with-chrono toolset=gcc link=static 
}

cd $WS_ROOT/3pl/sources/

if [ ! -d `basename ${POCO_VERSION} .tar.gz` ]
then 
    if [ ! -f $POCO_VERSION ]
    then
        wget http://cfhcable.dl.sourceforge.net/project/boost/boost/1.60.0/${POCO_VERSION}
        wget https://pocoproject.org/releases/poco-1.9.0/${POCO_VERSION}
        tar zxf $POCO_VERSION
    else
        tar zxf $POCO_VERSION
    fi
fi

install
post_handle 

