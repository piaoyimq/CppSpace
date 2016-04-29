cmake_minimum_required(VERSION 3.0.1)

if(NOT CMAKE_TOOLCHAIN_FILE)
    message(FATAL_ERROR "You must specify CMAKE_TOOLCHAIN_FILE")
endif()

if (DEFINED ENV{CCACHE})
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE $ENV{CCACHE})
endif()

if(NOT BASE_PLATFORM)
    set(BASE_PLATFORM ${PLATFORM})
endif()

# The Ninja generator (-G Ninja) does not know how to build archives
# in pieces, so response files must be used to deal with this
set(CMAKE_NINJA_FORCE_RESPONSE_FILE 1)

if(${PLATFORM} STREQUAL "Linux_x86")
elseif(${PLATFORM} STREQUAL "Linux_mips")
else()
    message(FATAL_ERROR "Unknown platform: ${PLATFORM}")
endif()

include_directories(BEFORE SYSTEM
#    ${OPENSSL_DIR}/include #piaoyimq
)

# Common include directories
include_directories(
 #   ${OMROOT}/LangCpp/osconfig/${PLATFORM}
 #   ${OMROOT}/LangCpp
 #   ${OMROOT}/LangCpp/oxf
)

include(CMakeParseArguments)

# Function for conditionally adding subdirectory based on platform
function(cpp_add_platform_subdirectory directory platforms)
    list(FIND platforms ${PLATFORM} index)
    if (${index} GREATER -1)
        message(STATUS "Adding ${directory} for platform ${PLATFORM}")
        add_subdirectory(${directory})
    else()
        message(STATUS "Skipping ${directory} for platform ${PLATFORM}")
    endif()
endfunction()

if(NOT TARGET all_esc)
    add_custom_target(all_esc)
endif()

macro(cpp_add_sourcefiles)
    foreach(arg ${ARGN})
        if (arg MATCHES "/$")
            file(GLOB_RECURSE SRCS "${arg}*.cpp" "${arg}*.c" "${arg}*.cc" "${arg}*.h")
            foreach(src ${SRCS})
                set(use 1)
                foreach(excl ${ARG_SRC_EXCLUDE})
                    if(src MATCHES "${excl}")
                        #Use for debugging: message(STATUS "Excluding ${src} from ${target}")
                        set(use 0)
                        break()
                    endif()
                endforeach()
                if(use AND NOT src MATCHES "\\.gen\\.(cpp|h)$")
                    list(APPEND SRC_FILES ${src})
                endif()
            endforeach()
        else()
            list(APPEND SRC_FILES "${arg}")
        endif()
    endforeach()
endmacro()

macro(cpp_arg_parse)
    cmake_parse_arguments(ARG "LINUX;TEST;NOTEST;NOTESTRUN;MIPS;NOMIPS" "BASE_DIR" "SRC;SRC_EXCLUDE;INCLUDE;INCLUDE_BEFORE;COMPILE_DEFS;PUBLIC;PRIVATE;LABEL;Linux_x86;Linux_mips" ${ARGN})

    if(ARG_LINUX AND NOT TARGET_OS STREQUAL "Linux")
        return()
    elseif(ARG_NOMIPS AND PLATFORM STREQUAL "Linux_mips")
        return()
    elseif(ARG_MIPS AND NOT PLATFORM STREQUAL "Linux_mips")
        return()
    elseif(ARG_NOTEST AND BUILD_TEST)
        return()
    elseif(ARG_TEST AND NOT BUILD_TEST)
        return()
    elseif((DEFINED ENV{BUILD_ONLY_LI}) AND (NOT (PROJECT_NAME STREQUAL "EPG_INTERNAL")))
        return()
    endif()

    if(ARG_BASE_DIR)
        list(APPEND ARG_INCLUDE "${PROJECT_SOURCE_DIR}/${ARG_BASE_DIR}")
    else()
        if(NOT ARG_INCLUDE)
            list(APPEND ARG_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}")
        endif()
    endif()

    if(NOT ARG_SRC)
        set(ARG_SRC "./")
    endif()

    set(PLATFORM_SPECIFIC_APP_LIBS ${ARG_${BASE_PLATFORM}})
endmacro()

function(cpp_add_lib target)
    cpp_arg_parse(${ARGN})

    set(TARGET ${target})
    set(TARGET_NAME "${TARGET}_${PLATFORM}")
    set(TARGET_NAME "${TARGET_NAME}" PARENT_SCOPE)

    cpp_add_sourcefiles(${ARG_SRC})

    add_library(${TARGET_NAME}
        ${SRC_FILES}
    )

    if(DEPS)
        add_dependencies(${TARGET_NAME}
            ${DEPS}
        )
    endif()

    add_library(${TARGET} ALIAS ${TARGET_NAME})

    target_link_libraries(${TARGET_NAME}
    PUBLIC
        ${ARG_PUBLIC}
        ${PLATFORM_SPECIFIC_APP_LIBS}
    PRIVATE
        ${ARG_PRIVATE}
    )

    target_compile_definitions(${TARGET_NAME}
    PRIVATE
        -DCOMPONENT_NAME="lib${TARGET}"
        ${ARG_COMPILE_DEFS}
    )

    if(ARG_INCLUDE_BEFORE)
        target_include_directories(${TARGET_NAME}
            BEFORE
            PUBLIC
                ${ARG_INCLUDE_BEFORE}
        )
    endif()

    target_include_directories(${TARGET_NAME}
    PUBLIC
        ${ARG_INCLUDE}
    )
endfunction()

#New unittests shall be added with this function
function(cpp_add_executable target)
    cpp_arg_parse(${ARGN})

    set(TARGET ${target})
    set(TARGET_NAME "${TARGET}_${PLATFORM}.elf")
    set(TARGET_NAME "${TARGET_NAME}" PARENT_SCOPE)

    cpp_add_sourcefiles(${ARG_SRC})

    add_executable(${TARGET_NAME}
        ${SRC_FILES}
    )

    if(DEPS)
        add_dependencies(${TARGET_NAME}
            ${DEPS}
        )
    endif()

    add_executable(${TARGET} ALIAS ${TARGET_NAME})

    target_link_libraries(${TARGET_NAME}
    PUBLIC
        ${ARG_PUBLIC}
        ${PLATFORM_SPECIFIC_APP_LIBS}
    PRIVATE
        ${ARG_PRIVATE}
        ${PLATFORM_LIBS}
    )

    if(ARG_TEST AND NOT ARG_NOTESTRUN)
        set_target_properties(${TARGET_NAME}
            PROPERTIES
                RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin/${TARGET}"

        )
        add_test(NAME ${TARGET} COMMAND $<TARGET_FILE:${TARGET_NAME}> WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/bin/${TARGET})
        if(ARG_LABEL)
            set_tests_properties(${TARGET} PROPERTIES LABELS "${ARG_LABEL}")
        endif()
    endif()

    target_compile_definitions(${TARGET_NAME}
    PRIVATE
        -DCOMPONENT_NAME="${TARGET}"
        ${ARG_COMPILE_DEFS}
    )

    if(ARG_INCLUDE_BEFORE)
        target_include_directories(${TARGET_NAME}
            BEFORE
            PUBLIC
                ${ARG_INCLUDE_BEFORE}
        )
    endif()

    target_include_directories(${TARGET_NAME}
    PUBLIC
        ${ARG_INCLUDE}
    )

    if(ARG_LABEL)
        foreach(LABEL ${ARG_LABEL})
            if(NOT TARGET ${LABEL})
                add_custom_target(${LABEL})
            endif()
            add_dependencies(${LABEL} ${TARGET_NAME})
        endforeach()
    endif()
endfunction()


