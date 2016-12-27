add_compile_options(
    -Wcast-qual
)


cpp_add_executable("boost_ut_exampleTest"
    #TEST LINUX
    SRC
       ./
 
    PUBLIC
	boost_unit_test_framework
)



