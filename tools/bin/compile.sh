#!/bin/bash

#ulimit -a
#ulimit -c unlimited
set -x
INC_PATH="-I $BASE_PATH/boost_1_60_0/include"
LIB_PATH="-L $BASE_PATH/boost_1_60_0/lib"
LIB="-lpthread -lboost_serialization -lboost_system -lboost_filesystem"
LIB_SO_PATH="-Wl,-rpath,$BASE_PATH/boost_1_60_0/lib/"

g++ -g -std=c++0x "$@" $INC_PATH $LIB_PATH $LIB $LIB_SO_PATH
