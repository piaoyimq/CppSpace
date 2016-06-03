#!/bin/bash

rm -rf a.out
g++ -std=c++0x $1 -lboost_serialization -lpthread
