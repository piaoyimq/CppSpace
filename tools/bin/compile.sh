#!/bin/bash

#ulimit -a
#ulimit -c unlimited
set -x
#INC="-I /opt/boost1.6/include"
#LIB="-L /opt/boost1.6/lib -lpthread -lboost_serialization -lboost_system"
LIB="-lpthread -lboost_serialization -lboost_system"


g++ -g -std=c++0x "$@" $INC $LIB
