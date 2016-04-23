#!/bin/bash

rm -rf *.log*
rm -rf log
rm -rf log-maint-test
rm -rf ../*.log

g++ -g LogMainTest.cpp Log.cpp -lpthread -o log-main-test
