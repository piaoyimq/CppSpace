#!/usr/bin/bash

rm -rf *.log*

g++ MainTest.cpp Log.cpp -lpthread -o log
