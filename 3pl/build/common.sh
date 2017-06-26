#!/bin/bash
export PACKAGE_PATH=$WS_ROOT/staging/packages/${platform}/$1 
export SRC_PATH=$WS_ROOT/3pl/sources/$1
export OUTPUT_PATH=$WS_ROOT/3pl/output/$1
export INCLUDE_PATH="usr/local/include"
export LIB_PATH="usr/local/lib"


mkdir_compile_time_path()
{
    mkdir -p $WS_ROOT/staging/${platform}/${INCLUDE_PATH}
    mkdir -p $WS_ROOT/staging/${platform}/${LIB_PATH}
}


mkdir_runtime_path()
{
    mkdir -p $PACKAGE_PATH/${INCLUDE_PATH} 
    mkdir -p $PACKAGE_PATH/${LIB_PATH}
}


pre_cmake()
{
    [ -f ${PACKAGE_PATH}*.tar.gz ] && echo -e "$1 library already exist."  && exit 0

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

