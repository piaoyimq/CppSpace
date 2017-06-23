#!/bin/bash
set -ex

PACKAGE_PATH=$WS_ROOT/staging/packages/${platform}/googletest 
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

#install compile time resources
mkdir -p $WS_ROOT/staging/${platform}/${INCLUDE_PATH}
mkdir -p $WS_ROOT/staging/${platform}/${LIB_PATH}
cp lib*.a $WS_ROOT/staging/${platform}/${LIB_PATH}
cp -r $GOOGLETEST_SRC_PATH/include/* $WS_ROOT/staging/${platform}/${INCLUDE_PATH} 
cp gtest/lib*.a $WS_ROOT/staging/${platform}/${LIB_PATH}
cp -r $GOOGLETEST_SRC_PATH/../googletest/include/* $WS_ROOT/staging/${platform}/${INCLUDE_PATH} 


#install run time resources
mkdir -p $PACKAGE_PATH/${INCLUDE_PATH}
mkdir -p $PACKAGE_PATH/${LIB_PATH}

cp -r $GOOGLETEST_SRC_PATH/include/* $PACKAGE_PATH/${INCLUDE_PATH}
cp lib*.a $PACKAGE_PATH/${LIB_PATH}
cp -r $GOOGLETEST_SRC_PATH/../googletest/include/* $PACKAGE_PATH/${INCLUDE_PATH}
cp gtest/lib*.a $PACKAGE_PATH/${LIB_PATH}

tar -Pzcvf ${PACKAGE_PATH}-`git HEAD`.tar.gz $PACKAGE_PATH > /dev/null

rm -rf $GOOGLETEST_OUTPUT_PATH
rm -rf $PACKAGE_PATH
