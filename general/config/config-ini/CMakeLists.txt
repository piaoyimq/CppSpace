add_compile_options(
    -Wcast-qual
)


cpp_add_lib("configini"
    #TEST LINUX
    SRC
        src/

    PUBLIC
        pthread
)


cpp_add_executable(ConfIniTest
    #TEST LINUX
    BASE_DIR "general/config/config-ini/test"
    SRC
        test/
      
    PUBLIC
        configini	   
	googletest
)

