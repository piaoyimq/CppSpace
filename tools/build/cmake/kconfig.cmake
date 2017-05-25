#-------------------------------------------------------------------------------
# Read and parse kconfig config
#
# - Returns all defines in KCONFIG_DEFINES
#
# - Sets CMake variables corresponding to definition in called scope
#   The buildsystem can react on kconfig settings
#   Naming is: CONFIG_<defines in kconfig>
#
# Search for kconfig settings in precedence order:
# 1. Defined KCONFIG_CONFIG (supplied by user via -DKCONFIG_CONFIG=...)
# 2. ENV defined KCONFIG_CONFIG (supplied by user as environment variable)



function(parse_kconfig)
    #set(KCONFIG_CONFIG_LOCAL  "$ENV{WS_ROOT}/tools/build/kconfig/.config")
    set(KCONFIG_CONFIG_DEFAULT "$ENV{WS_ROOT}/tools/build/kconfig/kconfig_default.config")
    
    if(DEFINED KCONFIG_CONFIG)
        if(EXISTS ${KCONFIG_CONFIG})
            set(FILE "${KCONFIG_CONFIG}")
        else()
            message(FATAL_ERROR "Unable to find file KCONFIG_CONFIG=${KCONFIG_CONFIG}")
        endif()
    elseif(DEFINED ENV{KCONFIG_CONFIG})
        if(EXISTS $ENV{KCONFIG_CONFIG})
            set(FILE "$ENV{KCONFIG_CONFIG}")
        else()
            message(FATAL_ERROR "Unable to find file KCONFIG_CONFIG=$ENV{KCONFIG_CONFIG}")
        endif()
    elseif(EXISTS ${KCONFIG_CONFIG_LOCAL})
        set(FILE "${KCONFIG_CONFIG_LOCAL}")
    elseif(EXISTS ${KCONFIG_CONFIG_DEFAULT})
        set(FILE "${KCONFIG_CONFIG_DEFAULT}")
    else()
        message(STATUS "No kconfig found")
        return()
    endif()

    # Read and convert file into a CMake list
    message(STATUS "Using kconfig file: ${FILE}")
    FILE(READ "${FILE}" DATA)
    string(REGEX REPLACE ";" "\\\\;" DATA "${DATA}")
    string(REGEX REPLACE "\n" ";" DATA "${DATA}")

    set(KCONFIG_DEFINES)
    foreach(LINE ${DATA})

        # Parse defines
        if (LINE MATCHES ".+=[ ]*y.*" AND NOT LINE MATCHES "^[ \t]*#")
            string(REGEX REPLACE "=[ ]*y.*" "" DEFINE "${LINE}")
            string(STRIP "${DEFINE}" DEFINE)
            set("${DEFINE}" 1 PARENT_SCOPE)
            list(APPEND KCONFIG_DEFINES "-D${DEFINE}")
        endif()

         if (LINE MATCHES ".+=[ ]*[0-9]+" AND NOT LINE MATCHES "^[ \t]*#")

            # Variable name
            string(REGEX REPLACE "=.*" "" NAME "${LINE}")
            string(STRIP "${NAME}" NAME)

            # Variable value
            string(REGEX REPLACE ".+=" "" VALUE "${LINE}")
            string(REPLACE "\"" "" VALUE ${VALUE})
            string(STRIP "${VALUE}" VALUE)
            list(APPEND KCONFIG_DEFINES "-D${NAME}=${VALUE}")
           endif()
           
        # Parse variables
        if (LINE MATCHES ".+=[ ]*\".*\"" AND NOT LINE MATCHES "^[ \t]*#")

            # Variable name
            string(REGEX REPLACE "=.*" "" NAME "${LINE}")
            string(STRIP "${NAME}" NAME)

            # Variable value
            string(REGEX REPLACE ".+=" "" VALUE "${LINE}")
            string(REPLACE "\"" "" VALUE ${VALUE})
            string(STRIP "${VALUE}" VALUE)
            
            # Expand environment variables in value
            string(REGEX MATCHALL "\\$\\{[a-zA-Z0-9_]+\\}" ENV_MATCHES "${VALUE}")
            foreach(MATCH ${ENV_MATCHES})
                set(ENV_NAME ${MATCH})
                string(REPLACE "\${" "" ENV_NAME "${ENV_NAME}")
                string(REPLACE "}" "" ENV_NAME "${ENV_NAME}")
                string(REPLACE "${MATCH}" "$ENV{${ENV_NAME}}" VALUE "${VALUE}")
            endforeach()
            
            set("${NAME}" ${VALUE} PARENT_SCOPE)
            
            #DEBUG
            #message(STATUS "${NAME} => ${VALUE}")
            
            list(APPEND KCONFIG_DEFINES "-D${NAME}=\"${VALUE}\"")
            
        endif()
    endforeach()
    
    set(KCONFIG_DEFINES ${KCONFIG_DEFINES} PARENT_SCOPE)
endfunction()


# Do the parsing
parse_kconfig()
if(KCONFIG_DEFINES)
    message(STATUS "Using kconfig defines: ${KCONFIG_DEFINES}")
    add_definitions(${KCONFIG_DEFINES})
endif()
