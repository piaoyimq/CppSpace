#!/bin/bash
set -ex

source $WS_ROOT/3pl/build/common.sh $1

pre_cmake $1


#install run time resources
cp -rf $SRC_PATH/googlemock/include/* $RUN_TIME_INCLUDE_PATH
cp googlemock/lib*.a $RUN_TIME_LIB_PATH
cp -rf $SRC_PATH/googletest/include/* $RUN_TIME_INCLUDE_PATH
cp -rf googlemock/gtest/lib*.a $RUN_TIME_LIB_PATH

post_handle
