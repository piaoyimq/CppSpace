set(TARGET_OS "Linux")
# This should be set to where we actually expect to deploy the software.  For
# now it doesn't matter since we do not deploy any dynamic libraries on these
# targets yet.
set(CMAKE_INSTALL_PREFIX "/usr/local")

add_definitions(
    -DLINUX_TARGET
    -DUSE_IOSTREAM

    # Classic
    -DSDK_VERSION="unknown"
    -DGDB_DEBUG
)

set(CMAKE_C_FLAGS "-fdebug-prefix-map=$ENV{WS_ROOT}=. -pipe" CACHE STRING "C compiler flags")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}" CACHE STRING "C++ compiler flags")

if(USE_AUTOMOCK)
    # Temporary solution, needs to be moved to staging
    set(OPENSSL_DIR "/home/ehsberl/openssl/openssl-1.0.2d")
else()
    #set(OPENSSL_DIR "$ENV{OPENSSL_STAGING}/linux-x86")#piaoyimq
endif()

find_program(CMAKE_STRIP strip)
