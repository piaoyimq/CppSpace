if(CONFIG STREQUAL "Linux_x86_64.automock")
  set(AUTOMOCK_EXT ".automock")
endif()



###
# 3pp overlays.
include_directories(SYSTEM $ENV{WS_ROOT}/staging/${PLATFORM}${AUTOMOCK_EXT}/${CMAKE_INSTALL_PREFIX}/include)
#message("3pl include: $ENV{WS_ROOT}/staging/${PLATFORM}${AUTOMOCK_EXT}/${CMAKE_INSTALL_PREFIX}/include")

set(TPP_GLOBAL_LINK_FLAGS)

# The path to the per-target staging directory including the installation
# prefix.
set(TPP_STAGING_PREFIX_DIR "$ENV{WS_ROOT}/staging/${PLATFORM}${AUTOMOCK_EXT}/${CMAKE_INSTALL_PREFIX}")

# Set static library search path.
list(APPEND TPP_GLOBAL_LINK_FLAGS "-L${TPP_STAGING_PREFIX_DIR}/lib")
#message("TPP_STAGING_PREFIX_DIR: ${TPP_STAGING_PREFIX_DIR}/lib")

# Set dynamic library search path during linking.
list(APPEND TPP_GLOBAL_LINK_FLAGS "-Wl,-rpath-link,${TPP_STAGING_PREFIX_DIR}/lib")
# Set dynamic library search path during runtime.
list(APPEND TPP_GLOBAL_LINK_FLAGS "-Wl,-rpath,${CMAKE_INSTALL_PREFIX}/lib")

string(REPLACE ";" " "
    TPP_GLOBAL_LINK_FLAGS "${TPP_GLOBAL_LINK_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${TPP_GLOBAL_LINK_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINK_FLAGS} ${TPP_GLOBAL_LINK_FLAGS}")

###
# Target-specific settings.
function(tpp_target_link_flags target_dir outLinkFlags)
    set(TPP_TARGET_LINK_FLAGS)

    if(PLATFORM STREQUAL "Linux_x86_64")
        # For design test targets add the relative path from the build/bin directory
        # since the executables won't be installed in the actual prefix directory.
        file(RELATIVE_PATH TPP_RELATIVE_LIB_DIR
            "${target_dir}" "${TPP_STAGING_PREFIX_DIR}/lib")
        list(APPEND TPP_TARGET_LINK_FLAGS "-Wl,-rpath,\\\$ORIGIN/${TPP_RELATIVE_LIB_DIR}")
    elseif(PLATFORM STREQUAL "Linux_elc")
        # For Elc we currently put the shared objects in the same directory as the executables
        list(APPEND TPP_TARGET_LINK_FLAGS "-Wl,-rpath,\\\$ORIGIN")
    endif()

    string(REPLACE ";" " "
        TPP_TARGET_LINK_FLAGS "${TPP_TARGET_LINK_FLAGS}")
    set(${outLinkFlags} ${TPP_TARGET_LINK_FLAGS} PARENT_SCOPE)
endfunction()

###
# Package-specific settings.

# Protobuf compiler path.
set(PROTOBUF_PROTOC_EXECUTABLE "$ENV{WS_ROOT}/staging/${PLATFORM}${AUTOMOCK_EXT}/${CMAKE_INSTALL_PREFIX}/bin/protoc")
