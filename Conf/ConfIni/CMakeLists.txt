add_compile_options(
    -Wcast-qual
)


cpp_add_lib("confini"
    #TEST LINUX
    SRC
        src/

    PUBLIC
        pthread
)


cpp_add_executable(ConfIniTest
    #TEST LINUX
    BASE_DIR "Conf/ConfIni/test"
    SRC
        test/
      
    PUBLIC
        confini	   
	googletest
)

