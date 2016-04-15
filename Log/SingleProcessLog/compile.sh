#!/bin/bash

rm -rf *.log*
rm -rf log
rm -rf log-maint-test
rm -rf ../*.log

g++ LogMainTest.cpp Log.cpp -lpthread -o log-main-test
