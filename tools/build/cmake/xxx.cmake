###
# xxx overlays.
#include_directories(SYSTEM $ENV{MOBILEGW_STAGING}/${PLATFORM}/${CMAKE_INSTALL_PREFIX}/include)#piaoyimq
# Set static library search path.
set(CMAKE_EXE_LINKER_FLAGS
  "${CMAKE_EXE_LINKER_FLAGS} -L$ENV{MOBILEGW_STAGING}/${PLATFORM}/${CMAKE_INSTALL_PREFIX}/lib")
# Set dynamic library search path during linking.
set(CMAKE_EXE_LINKER_FLAGS
  "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath-link -Wl,$ENV{MOBILEGW_STAGING}/${PLATFORM}/${CMAKE_INSTALL_PREFIX}/lib")
# Set dynamic library search path during runtime.
set(CMAKE_EXE_LINKER_FLAGS
  "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath -Wl,${CMAKE_INSTALL_PREFIX}/lib")

###
# Package-specific settings.

# Protobuf compiler path.
set(PROTOBUF_PROTOC_EXECUTABLE "$ENV{MOBILEGW_STAGING}/Linux_x86/${CMAKE_INSTALL_PREFIX}/bin/protoc")
