#!/bin/bash

function pre_cmake()
{

PACKAGE_PATH=$WS_ROOT/staging/packages/${platform}/$1 
GOOGLETEST_SRC_PATH=$WS_ROOT/3pl/sources/googletest/googlemock
GOOGLETEST_OUTPUT_PATH=$WS_ROOT/3pl/output/googlemock
INCLUDE_PATH="usr/local/include"
LIB_PATH="usr/local/lib"

[ -d $WS_ROOT/staging/${platform}/${INCLUDE_PATH}/gtest ] && [ -f ${PACKAGE_PATH}*.tar.gz ] && echo -e "googletest library already exist."  && exit 0

mkdir -p  $GOOGLETEST_SRC_PATH
mkdir -p $GOOGLETEST_OUTPUT_PATH

cd $GOOGLETEST_OUTPUT_PATH

cmake $GOOGLETEST_SRC_PATH 
make 

}

function post_cmake()
{
}

