#!/bin/bash
set -e

err_exit()                                                                                                                                                               
{
    echo -e "Build 3pl: $2 failed" >&2
    exit $1
}
 

function install_log()
{
    echo -e "Build 3pl: $1 ..."
    echo -e "install log: $WS_ROOT/3pl/output/log/install-$1.log "
}


function install()
{
    install_log $1
    start=$(date "+%s")
    $WS_ROOT/3pl/build/install-$1.sh > $WS_ROOT/3pl/output/log/install-$1.log 2>&1 || err_exit 10 $1
    now=$(date "+%s")
    time=$((now-start))
    echo -e "Build 3pl: $1 successful ${time}s"
}


install "googletest"
