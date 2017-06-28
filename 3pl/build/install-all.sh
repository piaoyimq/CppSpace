#!/bin/bash
#This scprit called by build.sh
set -e
export PLATFORM=$1
LOG_PATH_PREFIX=$WS_ROOT/3pl/output/log/install-$PLATFORM


err_exit()                                                                  
{
    echo -e "Build 3pl: $2 failed" >&2
    exit $1
}
 

install_log()
{
    mkdir -p $WS_ROOT/3pl/output/log
    echo -e "Build 3pl/$1 ..."
    echo -e "Build 3pl/$1 log: $LOG_PATH_PREFIX-$1.log"
}


install_common_part()
{
 echo ""
}


install()
{
    install_log $1
    start=$(date "+%s")
    $WS_ROOT/3pl/build/install-$1.sh $1> $LOG_PATH_PREFIX-$1.log 2>&1 || err_exit 10 $1
    now=$(date "+%s")
    time=$((now-start))
    echo -e "Build 3pl/$1 successful ${time}s"
}


install "googletest"&
install "boost"&
wait
