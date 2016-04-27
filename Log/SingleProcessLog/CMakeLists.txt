add_compile_options(
    -Wcast-qual
)

add_lib("singleprocesslog"
    SRC
        src/

    INCLUDE
        include/

    PRIVATE
    #support
)
