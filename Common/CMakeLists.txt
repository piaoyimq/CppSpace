add_compile_options(
    -Wcast-qual
)




cpp_add_lib("common"
    #TEST LINUX
    SRC
        src/Common.cpp

    PUBLIC
        pthread
)


cpp_add_executable(CommonTest
    #TEST LINUX
    BASE_DIR "Common/test"
    SRC
        test/
      
    PUBLIC
        common	   
)

