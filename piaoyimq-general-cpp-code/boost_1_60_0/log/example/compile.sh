
set -x
g++ -g -std=c++0x $1 -I $BASE_PATH/boost_1_60_0/include -DBOOST_LOG_DYN_LINK -DBOOST_LOG_USE_NATIVE_SYSLOG -L $BASE_PATH/boost_1_60_0/lib -lpthread -lboost_system -lboost_log_setup  -lboost_log  -lboost_thread -lboost_locale -Wl,-rpath,$BASE_PATH/boost_1_60_0/lib/
