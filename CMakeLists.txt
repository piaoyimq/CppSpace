cmake_minimum_required(VERSION 3.0.1)
project(EPG)

if (USE_COVERAGE)
    message(STATUS "Building with coverage for ${PLATFORM}.")
    add_compile_options("--coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
else()
    message(STATUS "No coverage for ${PLATFORM}.")
endif()

if (USE_ASAN)
    message(STATUS "Building with ASAN for ${PLATFORM}.")
    add_definitions(-DUSE_ASAN)
    add_compile_options("-Wno-unused-but-set-variable" "-Wno-maybe-uninitialized" "-fsanitize=address")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libasan -fsanitize=address")
else()
    add_compile_options(-Werror)
endif()

if (USE_GPROF)
    message(STATUS "Building with gprof profiling support for ${PLATFORM}.")
    add_compile_options("-pg")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
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

include(${EPG_SOURCE_DIR}/tools/build/cmake/common.cmake)
if(NOT PLATFORM STREQUAL "Linux_i586")
    include(${EPG_SOURCE_DIR}/tools/build/cmake/3pp.cmake)
    include(${EPG_SOURCE_DIR}/tools/build/cmake/libs.cmake)
endif()

set(CMAKE_CXX_LINK_EXECUTABLE "${LINKER_PREFIX} ${CMAKE_CXX_LINK_EXECUTABLE}")
set(CMAKE_C_LINK_EXECUTABLE "${LINKER_PREFIX} ${CMAKE_C_LINK_EXECUTABLE}")

# We need to change the ar command template since some EPG libraries have multiple
# object files with the same name the default is to replace objects with the same name
# which will cause linking errors later in the process
set(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> cq <TARGET> <LINK_FLAGS> <OBJECTS>")
set(CMAKE_CXX_ARCHIVE_APPEND "<CMAKE_AR> q <TARGET> <LINK_FLAGS> <OBJECTS>")

# General includes
include_directories(SYSTEM
    $ENV{BOOST_STAGING}/include
)
include_directories(
    application/Support/include
    framework/Support/include
    tools/test
)

# Strip unneeded so-lib dependencies for Linux_X, for example libsctp for EbmStreamRecorder_Linux_i586.elf.
if(TARGET_OS STREQUAL "Linux")
   set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--as-needed")
endif()

# General linker flags
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--build-id")
if(NOT CDPI_ARCH STREQUAL "ssr-rp")
    # Legacy build issue, needs investigation:
    # libcdpi.a and libheurndpi.a generates "no .eh_frame_hdr table will be created."
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--fatal-warnings")
endif()

# Handle kconfig
include(tools/build/cmake/kconfig.cmake)

# Handle SEOS-SDK
if(TARGET_OS STREQUAL "SEOS")
    include(tools/build/cmake/sdk.cmake)
endif()


if (PLATFORM STREQUAL "Linux_x86_64")
    enable_testing()
    add_subdirectory(tools/test/epg_portoverride)
endif()

# Rhapsody framework
add_subdirectory(tools/rhapsody)

find_library(LIB_BOOST_SYSTEM NAMES libboost_system.a PATHS $ENV{BOOST_STAGING}/${PLATFORM}/ NO_DEFAULT_PATH)
find_library(LIB_BOOST_FILESYSTEM NAMES libboost_filesystem.a PATHS $ENV{BOOST_STAGING}/${PLATFORM}/ NO_DEFAULT_PATH)
find_library(LIB_BOOST_REGEX NAMES libboost_regex.a PATHS $ENV{BOOST_STAGING}/${PLATFORM}/ NO_DEFAULT_PATH)

message(STATUS "Boost library ${LIB_BOOST_SYSTEM}")
message(STATUS "Boost library ${LIB_BOOST_FILESYSTEM}")
message(STATUS "Boost library ${LIB_BOOST_REGEX}")

# General warning flags
add_compile_options(
    # -Werror is defined above
    -Wall
    -Wextra
    -Wmissing-include-dirs
    -Wcast-align
    -Winvalid-pch

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


add_subdirectory(classic/src)
add_subdirectory(framework)
add_subdirectory(application)
#module.cmake is an artefact of the rhapsody migration and will eventually be
#removed. Add new subdirectories in their appropriate hierarchy under
#application, classic or framework.
include(module.cmake)

if(EXISTS "$ENV{MOBILEGW_INTERNAL}" AND IS_DIRECTORY "$ENV{MOBILEGW_INTERNAL}")
    add_subdirectory($ENV{MOBILEGW_INTERNAL} ${PROJECT_BINARY_DIR}/internal)
endif()

# CMake Eclipse project: Update .project file
if (MODIFY_ECLIPSE_PROJECT)
   add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/.project-modified
        COMMAND echo "-- Modify_project: ${CMAKE_BINARY_DIR}/.project file"
        COMMAND /app/epg/toside/scripts/modify_project.py -o .project .project
        COMMAND touch .project-modified
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        DEPENDS ${CMAKE_BINARY_DIR}/.project
    )
    add_custom_target(modify_eclipse ALL DEPENDS ${CMAKE_BINARY_DIR}/.project-modified)
endif()
