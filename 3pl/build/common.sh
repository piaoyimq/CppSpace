#!/bin/bash
export PACKAGE_PATH=$WS_ROOT/staging/packages/${PLATFORM}/$1 
export SRC_PATH=$WS_ROOT/3pl/sources/$1
export OUTPUT_PATH=$WS_ROOT/3pl/output/${PLATFORM}/$1
export RUN_TIME_INCLUDE_PATH=$PACKAGE_PATH/${INCLUDE_PATH}
export RUN_TIME_LIB_PATH=$PACKAGE_PATH/${LIB_PATH}


#set the gcc tools
if [ "Linux_x86_64" == $PLATFORM ]
then
    echo "Use default gcc tools"
elif [ "Linux_mips" == $PLATFORM ]
then
    export CC=${AUTOMOCK_GCC_PREFIX}gcc
    export CXX=${AUTOMOCK_GCC_PREFIX}g++
    #LD=
    #AR=
    #NM=
    #OBJCOPY=
    #OBJDUMP=
    #PKG_CONFIG_LIBDIR=
    #PKG_CONFIG_PATH=
    #CFLAGS=
    #CXXFLAGS=
    #LDFAGS=
    #RANLIB=
else
    echo "Unkonw platform"
    exit 1
fi


#mkdir_compile_time_path()
#{
#    mkdir -p $COMPILE_TIME_INCLUDE_PATH
#    mkdir -p $COMPILE_TIME_LIB_PATH
#}


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

    cmake -DCMAKE_TOOLCHAIN_FILE="$WS_ROOT/tools/build/cmake/platforms/${PLATFORM}.cmake" $SRC_PATH
    make 
    mkdir_runtime_path

}


post_cmake()
{
    #install compile time resources
    flock -w 300 $COMPILE_TIME_PATH -c "cp -fLrs $PACKAGE_PATH/* $COMPILE_TIME_PATH"

    tar -zcvf ${PACKAGE_PATH}-`git head`.tar.gz -C $PACKAGE_PATH .
#    rm -rf $OUTPUT_PATH
    rm -rf $PACKAGE_PATH
}

