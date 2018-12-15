########
# test #
########

if( Midge_ENABLE_TESTING )

    set( lib_dependencies
        Midge
    )
    
    set( programs
        test_typelength
        test_typeat
        test_typechain
        test_message
        test_buffer
    )
    
    pbuilder_executables( programs lib_dependencies )
    
    ##########
    # config #
    ##########
    
    
    set( midge_config_files
        config/test_types.json
        config/test_arrays.json
        config/test_objects.json
        config/test_comparison.json
    )
    
    pbuilder_install_config( ${midge_config_files} )

endif( Midge_ENABLE_TESTING )
    