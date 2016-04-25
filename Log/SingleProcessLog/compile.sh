#!/bin/bash

rm -rf *.log*
rm -rf log
rm -rf log-maint-test
rm -rf ../*.log


g++ -c  Log.cpp
ar -crv liblog.a  Log.o
mv liblog.a ../../build/lib

g++ -g LogMainTest.cpp ../../build/lib/liblog.a -lpthread -o log-main-test

rm -rf *.o *.h.gch

