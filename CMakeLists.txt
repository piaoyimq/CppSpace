cmake_minimum_required(VERSION 3.0.1)
project(CppSapce)

if (USE_COVERAGE)
    message(STATUS "Building with coverage for ${PLATFORM}.")
    add_compile_options("--coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
else()
    message(STATUS "No coverage for ${PLATFORM}.")
endif()

if (USE_DEBUG)
    message(STATUS "Building with unoptimized debug support for ${PLATFORM}.")
    STRING(REGEX REPLACE "-O." "-O0" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    STRING(REGEX REPLACE "-O." "-O0" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    add_compile_options("-funit-at-a-time") # Workaround for buggy warning in gcc 4.3.4 treated as error
    set(LINKER_PREFIX "${LINKER_PREFIX} -d")
endif()

# Increased execution protection
# -fstack-protector-strong should be used when we go to gcc 4.8
if (USE_SEC)
    message(STATUS "Building with enforced DEP,RELRO & Canaries for ${PLATFORM}.")
    add_definitions(-DUSE_SEC)
    add_compile_options("-fstack-protector")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fstack-protector")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}  -Wl,-z,now -Wl,-z,relro -Wl,-z,noexecstack")
endif()

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

include(${CppSapce_SOURCE_DIR}/tools/build/cmake/common.cmake)
if(NOT PLATFORM STREQUAL "Linux_i586")
    include(${CppSapce_SOURCE_DIR}/tools/build/cmake/xxx.cmake)
    include(${CppSapce_SOURCE_DIR}/tools/build/cmake/libs.cmake)
endif()

set(CMAKE_CXX_LINK_EXECUTABLE "${LINKER_PREFIX} ${CMAKE_CXX_LINK_EXECUTABLE}")
set(CMAKE_C_LINK_EXECUTABLE "${LINKER_PREFIX} ${CMAKE_C_LINK_EXECUTABLE}")

# We need to change the ar command template since some CppSapce libraries have multiple
# object files with the same name the default is to replace objects with the same name
# which will cause linking errors later in the process
set(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> cq <TARGET> <LINK_FLAGS> <OBJECTS>")
set(CMAKE_CXX_ARCHIVE_APPEND "<CMAKE_AR> q <TARGET> <LINK_FLAGS> <OBJECTS>")

# Strip unneeded so-lib dependencies for Linux_X, for example libsctp for EbmStreamRecorder_Linux_i586.elf.
if(TARGET_OS STREQUAL "Linux")
   set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--as-needed")
endif()

include(tools/build/cmake/3pl.cmake)
# General linker flags
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--build-id")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--fatal-warnings")

#set(BOOST_PATH "$ENV{WS_ROOT}/../boost_1_60_0")
set(DEFAULT_PATH "/usr")
set(CMAKE_SKIP_BUILD_RPATH FALSE) #set(CMAKE_INSTALL_RPATH "${BOOST_PATH/lib}")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L ${BOOST_PATH}/lib")


message("CMAKE_EXE_LINKER_FLAGS: ${CMAKE_EXE_LINKER_FLAGS}")

# Set dynamic library search path during linking.
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath-link -Wl,${BOOST_PATH}/lib")

# Set dynamic library search path during runtime.
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath -Wl,${BOOST_PATH}/lib")



# Handle kconfig
include(tools/build/cmake/kconfig.cmake)



# find library
#find_library(LIB_BOOST_SERIALIZATION NAMES libboost_serialization.a PATHS ${BOOST_PATH}/lib NO_DEFAULT_PATH)
#message(STATUS "Boost library ${LIB_BOOST_SERIALIZATION}")
#set(CMAKE_LIBRARY_PATH "${LIB_BOOST_SERIALIZATION}")



# General warning flags
add_compile_options(
  #  -Werror
  #  -Wall
    -Wextra
    -Wmissing-include-dirs
    -Wcast-align
    -Winvalid-pch
    -std=c++0x
    #    -Wl,-rpath, ${BOOST_PATH}/lib
    #The flags below are flags that we would like to introduce, but can't enable yet because there are still warnings left that need to be fixed first
    #DO NOT remove this list, we'll step by step uncomment these lines:
    #-Wcast-qual
    #-Wconversion
    #-Wdisabled-optimization
    #-Wdouble-promotion
    #-Wduplicated-cond  (GCC 6.0)
    #-Wformat=2
    #-Wfloat-equal
    #-Wlogical-op
    #-Wmissing-declarations
    #-Wmissing-format-attribute
    #-Wpointer-arith
    #-Wredundant-decls
    #-Wshadow
    #-Wsync-nand
    #-Wtrampolines
    #-Wundef
    #-Wunused-const-variable (GCC 5.2 ?)
    #-Wvla
    #-pedantic-errors
)

# C++ specific warning flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ")
#DO NOT remove below list, will be introduced soon
#  -Wctor-dtor-privacy
#  -Wnon-virtual-dtor
#  -Woverloaded-virtual
#  -Wsign-promo
#  -Wzero-as-null-pointer-constant (GCC 4.7.0)

# General includes
#include_directories(SYSTEM
#    ${BOOST_PATH}/include
#    #tools/test
#)


include_directories(BEFORE
    $ENV{WS_ROOT}/general
)

add_subdirectory(general/pq-common)

