#!/bin/bash

rm -rf a.out
g++ $1 -lboost_serialization -lpthread
