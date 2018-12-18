########
# test #
########

if( Midge_ENABLE_TESTING )

    set( lib_dependencies
        Midge
    )
    
    set( programs
        test_buffer
        test_message
        test_producer
        test_typelength
        test_typeat
        test_typechain
        test_typereplace
    )
    
    pbuilder_executables( programs lib_dependencies )
    
endif( Midge_ENABLE_TESTING )
    