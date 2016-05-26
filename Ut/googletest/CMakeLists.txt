add_compile_options(
    -Wcast-qual
)


cpp_add_lib("googletest"
    #TEST LINUX
    SRC
        src/

    INCLUDE
	include/
	./

    PUBLIC
        pthread
)



