# Cannot use the excellent jbmalloc with ASan
if(USE_ASAN)
    set(LINK_JBMALLOC)
else()
    set(LINK_JBMALLOC jbmalloc)
endif()

epg_add_lib("app_logging_platform"
    SEOS SSC

    SRC
        Logging/platform/seos/

    PUBLIC
        srvlib
)

epg_add_lib("app_logging_platform"
    LINUX

    SRC
        Logging/platform/elc/

    PUBLIC
        srvlib
)

epg_add_lib("app_logging"
    SSC

    SRC
        Logging/src/

    PUBLIC
        app
        app_logging_platform
        monotonic_time
        srvlib

        # Piaf
        logging
)

epg_add_lib("app"
    SSC

    SRC
        src/

    PUBLIC
        app_logging
        core
        memtracker
        monotonic_time
        oamframework
        platform
        srvlib

    SEOS_ssc
        ${LINK_JBMALLOC}
)
