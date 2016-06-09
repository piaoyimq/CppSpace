#!/bin/bash

#ulimit -a
#ulimit -c unlimited
rm -rf a.out
g++ -g -std=c++0x "$@" -lboost_serialization -lpthread
