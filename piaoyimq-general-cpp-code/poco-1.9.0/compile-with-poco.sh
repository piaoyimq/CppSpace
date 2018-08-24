#!/bin/bash

g++ $1 -I /opt/gss/poco_1_6/include/ -L /opt/gss/poco_1_6/lib/ -lPocoFoundation -Wl,-rpath,/opt/gss/poco_1_6/lib/
