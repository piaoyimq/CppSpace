add_compile_options(
    -Wcast-qual
)



cpp_add_lib("singleprocesslog"
    #TEST LINUX
    SRC
        src/

 #   INCLUDE
 #       include/

    PUBLIC
        pthread
        common
)


cpp_add_executable(SingleProcessLogTest
    #TEST LINUX
    BASE_DIR "general/log/single-process-log/test/"
    SRC
        test/
      
    PUBLIC
        singleprocesslog	   
	googletest
)

