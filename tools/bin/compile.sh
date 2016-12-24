#!/bin/bash

#ulimit -a
#ulimit -c unlimited
INC_PATH="-I $BASE_PATH/boost_1_60_0/include"
LIB_PATH="-L $BASE_PATH/boost_1_60_0/lib"
LIB="-lpthread -lboost_serialization -lboost_system -lboost_filesystem -lboost_container -lboost_unit_test_framework -lboost_graph -lboost_wserialization -lboost_program_options -lboost_log_setup -lboost_regex -lboost_log -lboost_atomic -lboost_thread -lboost_chrono -lboost_context -lboost_coroutine -lboost_date_time -lboost_exception -lboost_iostreams -lboost_locale -lboost_prg_exec_monitor -lboost_random -lboost_signals -lboost_system -lboost_test_exec_monitor -lboost_thread -lboost_timer -lboost_type_erasure -lboost_wave -lboost_wserialization"
#libboost_graph is property_tree
DEFINE_OPTIONS="-DBOOST_LOG_DYN_LINK -DBOOST_LOG_USE_NATIVE_SYSLOG"
LIB_SO_PATH="-Wl,-rpath,$BASE_PATH/boost_1_60_0/lib/"

set -x
g++ -O0 -g -std=c++0x "$@" $INC_PATH $DEFINE_OPTIONS $LIB_PATH $LIB $LIB_SO_PATH


