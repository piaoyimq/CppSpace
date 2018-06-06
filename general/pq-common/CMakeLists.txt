add_compile_options(
    -Wcast-qual
)


cpp_add_lib("pqcommon"
    #TEST LINUX
    SRC
        src/

    PUBLIC
        pthread
        boost_system 
        boost_log_setup 
        boost_log 
        boost_thread 
        boost_locale 
        boost_filesystem
)

cpp_add_lib("pqlog"
    #TEST LINUX
    SRC
        src/PocoLog.cpp

    PUBLIC
        PocoUtil 
        PocoFoundation
)

cpp_add_executable(CommonTest
    #TEST LINUX
    BASE_DIR "general/pq-common/test"
    SRC
        test/common-main-test.cpp
    PUBLIC
        pqcommon
        gtest      
        gtest_main
)


cpp_add_executable(LogTest
    #TEST LINUX
    BASE_DIR "general/pq-common/test"
    SRC
        test/log-main-test.cpp
      
    PUBLIC
        pqcommon
        gtest      
        gtest_main
)

cpp_add_executable(PocoLogTest
    #TEST LINUX
    BASE_DIR "general/pq-common/test"
    SRC
        test/PocoLogMainTest.cpp
      
    PUBLIC
        pqcommon
        pqlog
        gtest      
        gtest_main
)
