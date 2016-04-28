add_compile_options(
    -Wcast-qual
)

epg_add_lib("singleprocesslog"
    SRC
        src/

    INCLUDE
        include/

    PRIVATE
    #support
)
