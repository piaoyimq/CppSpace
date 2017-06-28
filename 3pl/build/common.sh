#!/bin/bash
export PACKAGE_PATH=$WS_ROOT/staging/packages/${PLATFORM}/$1 
export SRC_PATH=$WS_ROOT/3pl/sources/$1
export OUTPUT_PATH=$WS_ROOT/3pl/output/${PLATFORM}/$1
export INCLUDE_PATH="usr/local/include"
export LIB_PATH="usr/local/lib"
export COMPILE_TIME_INCLUDE_PATH=$WS_ROOT/staging/${PLATFORM}/${INCLUDE_PATH}
export COMPILE_TIME_LIB_PATH=$WS_ROOT/staging/${PLATFORM}/${LIB_PATH}
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
    tar -zcvf ${PACKAGE_PATH}-`git head`.tar.gz -C $PACKAGE_PATH .
#    rm -rf $OUTPUT_PATH
    rm -rf $PACKAGE_PATH
}

