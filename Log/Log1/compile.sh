#!/bin/bash

rm -rf *.log*
rm -rf log
rm -rf log-maint-test

g++ LogMainTest.cpp Log.cpp -lpthread -o log-main-test
