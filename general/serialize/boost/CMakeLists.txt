add_compile_options(
    -Wcast-qual
)


cpp_add_executable("demo"
    #TEST LINUX
    SRC
        demo.cpp
    PUBLIC
	    boost_serialization
)



