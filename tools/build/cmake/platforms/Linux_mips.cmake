set(PLATFORM "Linux_mips")
set(CDPI_ARCH "x86")
set(LI_PLATFORM ${PLATFORM})
set(BUILD_RP 1)
set(BUILD_SSC 1)

include(${CMAKE_CURRENT_LIST_DIR}/linux-common.cmake)

add_definitions(
    -DLINUX_TARGET_MIPS
)

# When we have converted everything to CMake, we will be
# able to enable this flag in order to reduce time for feedback loops during component/unit-testing
#set(BUILD_SHARED_LIBS ON)

set(CMAKE_C_FLAGS "-O1 -ggdb -fvar-tracking-assignments ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "-O1 -ggdb -fvar-tracking-assignments ${CMAKE_CXX_FLAGS}")
set(LINKER_PREFIX "do_link -c gcc")
