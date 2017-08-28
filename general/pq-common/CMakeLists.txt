add_compile_options(
    -Wcast-qual
)




cpp_add_lib("pqcommon"
    #TEST LINUX
    SRC
        src/

    PUBLIC
        pthread
)


cpp_add_executable(CommonTest
    #TEST LINUX
    BASE_DIR "general/pq-common/test"
    SRC
        test/
      
    PUBLIC
        pqcommon	   
	gtest
)

