#!/bin/bash
set -ex

source $WS_ROOT/3pl/build/common.sh $1

pre_cmake $1

#install compile time resources
cp googlemock/lib*.a $WS_ROOT/staging/${platform}/${LIB_PATH}
cp -r $SRC_PATH/googlemock/include/* $WS_ROOT/staging/${platform}/${INCLUDE_PATH} 
cp googlemock/gtest/lib*.a $WS_ROOT/staging/${platform}/${LIB_PATH}
cp -r $SRC_PATH/googletest/include/* $WS_ROOT/staging/${platform}/${INCLUDE_PATH} 


#install run time resources
cp -r $SRC_PATH/googlemock/include/* $PACKAGE_PATH/${INCLUDE_PATH}
cp googlemock/lib*.a $PACKAGE_PATH/${LIB_PATH}
cp -r $SRC_PATH/googletest/include/* $PACKAGE_PATH/${INCLUDE_PATH}
cp googlemock/gtest/lib*.a $PACKAGE_PATH/${LIB_PATH}

post_cmake
