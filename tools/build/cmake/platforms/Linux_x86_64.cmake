set(PLATFORM "Linux_x86_64")
set(CDPI_ARCH "x86")
set(LI_PLATFORM "Linux_mips")
set(BUILD_RP 1)
set(BUILD_SSC 1)
set(BUILD_TEST 1)

include(${CMAKE_CURRENT_LIST_DIR}/linux-common.cmake)

# When we have converted everything to CMake, we will be
# able to enable this flag in order to reduce time for feedback loops during component/unit-testing
#set(BUILD_SHARED_LIBS ON)

if (USE_AUTOMOCK)
    set(GCC_AUTOMOCK_COMPILE_FLAGS "-m32 -finstrument-functions -ffixed-ebp -maccumulate-outgoing-args -save-temps=obj -DAUTOMOCK")
    set(CMAKE_C_FLAGS "-O0 -ggdb -fvar-tracking-assignments ${GCC_AUTOMOCK_COMPILE_FLAGS} ${CMAKE_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "-O0 -ggdb -fvar-tracking-assignments -w ${GCC_AUTOMOCK_COMPILE_FLAGS} ${CMAKE_CXX_FLAGS}")
    set(LINKER_PREFIX "$ENV{WS_ROOT}/tools/build/do_link.sh -c 'gcc -m32'")
    set(BUILD_TEST 0)
else()
    set(CMAKE_C_FLAGS "-O1 -ggdb -fvar-tracking-assignments ${CMAKE_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "-O1 -ggdb -fvar-tracking-assignments ${CMAKE_CXX_FLAGS}")
    set(LINKER_PREFIX "$ENV{WS_ROOT}/tools/build/do_link.sh -c gcc")
endif()

if (USE_COVERAGE)
    set(LI_PLATFORM ${PLATFORM})
endif()

mark_as_advanced(CMAKE_STRIP)
