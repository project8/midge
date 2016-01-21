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
        test_ascii
        test_binary
        test_thread
        test_buffer
        test_lexer_reporter
        test_lexer_serializer
        test_lexer_compiler_reporter
        test_lexer_compiler_serializer
        test_builder
    )
    
    pbuilder_executables( programs lib_dependencies )
    
    ##########
    # config #
    ##########
    
    
    set( midge_config_files
        config/test_types.json
        config/test_arrays.json
        config/test_objects.json
        config/test_builder.json
        config/test_comparison.json
    )
    
    pbuilder_install_config( ${midge_config_files} )

endif( Midge_ENABLE_TESTING )
    