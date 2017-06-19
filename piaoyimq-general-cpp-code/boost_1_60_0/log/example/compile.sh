
set -x
g++ -g -std=c++0x $1 -I /home/ezhweib/workspace/boost_1_60_0/include -DBOOST_LOG_DYN_LINK -DBOOST_LOG_USE_NATIVE_SYSLOG -L /home/ezhweib/workspace/boost_1_60_0/lib -lpthread -lboost_system -lboost_log_setup  -lboost_log  -lboost_thread -lboost_locale -Wl,-rpath,/home/ezhweib/workspace/boost_1_60_0/lib/
