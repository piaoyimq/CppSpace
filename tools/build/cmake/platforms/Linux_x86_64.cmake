set(PLATFORM "Linux_x86_64")
set(CDPI_ARCH "x86_64")
set(BUILD_RP 1)
set(BUILD_SSC 1)
set(BUILD_TEST 1)

include(${CMAKE_CURRENT_LIST_DIR}/linux-common.cmake)


# When we have converted everything to CMake, we will be
# able to enable this flag in order to reduce time for feedback loops during component/unit-testing
#set(BUILD_SHARED_LIBS ON)

#set(USE_AUTOMOCK 1)
if (USE_AUTOMOCK)
    set(COMPILER_PREFIX "$ENV{AUTOMOCK_GCC_PREFIX}")
    set(COMPILER_ROOT "$ENV{AUTOMOCK_GCC_ROOT}")
    set(CMAKE_C_COMPILER "${COMPILER_PREFIX}gcc")
    set(CMAKE_CXX_COMPILER "${COMPILER_PREFIX}g++")

    set(GCC_AUTOMOCK_COMPILE_FLAGS "-m32 -finstrument-functions -ffixed-ebp -maccumulate-outgoing-args -save-temps=obj -DAUTOMOCK")
    set(CMAKE_C_FLAGS "-O0 -ggdb -fvar-tracking-assignments ${GCC_AUTOMOCK_COMPILE_FLAGS} ${CMAKE_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "-O0 -ggdb -fvar-tracking-assignments -w ${GCC_AUTOMOCK_COMPILE_FLAGS} ${CMAKE_CXX_FLAGS}")
    # intercept .so linker
    #set(CMAKE_CXX_CREATE_SHARED_LIBRARY "$ENV{WS_ROOT}/classic/src/ggsnc/ctframework/gen-stubs/capture_build.py ${COMPILER_PREFIX}gcc -m32 <CMAKE_SHARED_LIBRARY_CXX_FLAGS> <LANGUAGE_COMPILE_FLAGS> <LINK_FLAGS> <CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS> <SONAME_FLAG><TARGET_SONAME> -o <TARGET> <OBJECTS> <LINK_LIBRARIES>")
    set(BUILD_TEST 0)
elseif (USE_CLANG)
    set(COMPILER_PREFIX "$ENV{LINUX_CLANG_PREFIX}")
    set(COMPILER_ROOT "$ENV{LINUX_CLANG_ROOT}")
    if (USE_SCANBUILD)
        set(CMAKE_C_COMPILER "$ENV{CC}")
        set(CMAKE_CXX_COMPILER "$ENV{CXX}")
    else()
        set(CMAKE_C_COMPILER "${COMPILER_PREFIX}clang")
        set(CMAKE_CXX_COMPILER "${COMPILER_PREFIX}clang++")
    endif()
    set(CMAKE_C_FLAGS "-O1 -ggdb ${CMAKE_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "-O1 -ggdb ${CMAKE_CXX_FLAGS}")
    set(LINKER_PREFIX "do_link -c '${COMPILER_PREFIX}clang' -s '${COMPILER_PREFIX}strip'")
else()
    set(CMAKE_C_FLAGS "-O1 -ggdb -fvar-tracking-assignments ${CMAKE_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "-O1 -ggdb -fvar-tracking-assignments ${CMAKE_CXX_FLAGS}")
    set(LINKER_PREFIX "$ENV{WS_ROOT}/tools/build/do_link.sh -c gcc")
    # set(CMAKE_AR gcc-ar CACHE FILEPATH "Archiver")
    # set(CMAKE_RANLIB gcc-ranlib CACHE FILEPATH "Archive tool")
    # set(CMAKE_NM gcc-nm CACHE FILEPATH "Object symbol printer")
endif()


mark_as_advanced(CMAKE_STRIP)
