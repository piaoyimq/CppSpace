#!/bin/bash
export PACKAGE_PATH=$WS_ROOT/staging/packages/${platform}/$1 
export SRC_PATH=$WS_ROOT/3pl/sources/$1
export OUTPUT_PATH=$WS_ROOT/3pl/output/${platform}/$1
export INCLUDE_PATH="usr/local/include"
export LIB_PATH="usr/local/lib"
export COMPILE_TIME_INCLUDE_PATH=$WS_ROOT/staging/${platform}/${INCLUDE_PATH}
export COMPILE_TIME_LIB_PATH=$WS_ROOT/staging/${platform}/${LIB_PATH}
export RUN_TIME_INCLUDE_PATH=$PACKAGE_PATH/${INCLUDE_PATH}
export RUN_TIME_LIB_PATH=$PACKAGE_PATH/${LIB_PATH}


mkdir_compile_time_path()
{
    mkdir -p $COMPILE_TIME_INCLUDE_PATH
    mkdir -p $COMPILE_TIME_LIB_PATH
}


mkdir_runtime_path()
{
    mkdir -p $RUN_TIME_INCLUDE_PATH
    mkdir -p $RUN_TIME_LIB_PATH
}


check_library_exist()
{
    if [ -f ${PACKAGE_PATH}*.tar.gz ]
    then
        echo -e "$1 library already exist."
        exit 0
    fi
}


pre_cmake()
{
    check_library_exist $1

    mkdir -p $OUTPUT_PATH

    cd $OUTPUT_PATH

    cmake $SRC_PATH 
    make 
    mkdir_compile_time_path
    mkdir_runtime_path

}


post_cmake()
{
    cd $PACKAGE_PATH
    cd ..
    tar zcf $1-`git head`.tar.gz  $1
    rm -rf $OUTPUT_PATH
    rm -rf $PACKAGE_PATH
}

