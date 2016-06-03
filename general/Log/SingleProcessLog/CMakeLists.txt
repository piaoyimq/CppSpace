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
    BASE_DIR "common/Log/SingleProcessLog/test/"
    SRC
        test/
      
    PUBLIC
        singleprocesslog	   
)

