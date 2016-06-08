#!/bin/bash

rm -rf a.out
#g++ -std=c++0x "$@" -lboost_serialization -lpthread
g++  "$@" -lboost_serialization -lpthread
