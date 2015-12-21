########
# test #
########

set( midge_test_dependencies
    ${midge_library}
)

set( midge_test_directory
    test
)
set( midge_test_programs
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

midge_executables( test )

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
