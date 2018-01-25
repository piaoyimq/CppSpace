
set -x
g++ -g -std=c++0x $1 -I $BASE_PATH/CppSpace/staging/Linux_x86_64/usr/local/include -DBOOST_LOG_DYN_LINK -DBOOST_LOG_USE_NATIVE_SYSLOG -L $BASE_PATH/CppSpace/staging/Linux_x86_64/usr/local/lib -lpthread -lboost_system -lboost_log_setup  -lboost_log  -lboost_thread -lboost_locale -Wl,-rpath,$BASE_PATH/CppSpace/staging/Linux_x86_64/usr/local/lib/
