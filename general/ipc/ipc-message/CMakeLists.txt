add_compile_options(
    -Wcast-qual
)


cpp_add_lib("ipcmessage"
    #TEST LINUX
    SRC
        src/

    PUBLIC
	common
	boost_serialization
        pthread
)


cpp_add_executable(IpcMessageMain
    #TEST LINUX
    BASE_DIR "general/ipc/ipc-message/test"
    SRC
        test/
      
    PUBLIC
        ipcmessage	   
	googletest
)

