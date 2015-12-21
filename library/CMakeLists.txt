set( core_dir core )
set( bind_dir bindings )
set( init_dir initialization )
set( util_dir utility )

###########
# utility #
###########

set( midge_headers ${midge_headers}
	${util_dir}/macros.hh
	${util_dir}/initializer.hh
	${util_dir}/singleton.hh
	${util_dir}/error.hh
	${util_dir}/message.hh
	${util_dir}/ascii.hh
	${util_dir}/binary.hh
	${util_dir}/plot.hh
	${util_dir}/mutex.hh
	${util_dir}/thread.hh
	${util_dir}/types.hh
	${util_dir}/typenull.hh
	${util_dir}/typenode.hh
	${util_dir}/typelist.hh
	${util_dir}/typelength.hh
	${util_dir}/typeat.hh
	${util_dir}/typein.hh
	${util_dir}/typeif.hh
	${util_dir}/typechain.hh
)
set( midge_sources ${midge_sources}
	${util_dir}/error.cc
	${util_dir}/message.cc
	${util_dir}/ascii.cc
	${util_dir}/binary.cc
	${util_dir}/plot.cc
	${util_dir}/mutex.cc
	${util_dir}/thread.cc
)

set( midge_utility_test_dependencies
	utility
)
set( midge_utility_test_directory
	test
)
set( midge_utility_test_programs
	test_typelength
	test_typeat
	test_typechain
	test_message
	test_ascii
	test_binary
	test_thread
	test_buffer
)

midge_executables( utility_test )


########
# core #
########

set( midge_headers ${midge_headers}
	${core_dir}/coremsg.hh
	${core_dir}/stream.hh
	${core_dir}/_stream.hh
	${core_dir}/input.hh
	${core_dir}/_input.hh
	${core_dir}/output.hh
	${core_dir}/_output.hh
	${core_dir}/node.hh
	${core_dir}/_buffer.hh
	${core_dir}/_in.hh
	${core_dir}/_out.hh
	${core_dir}/producer.hh
	${core_dir}/consumer.hh
	${core_dir}/transformer.hh
	${core_dir}/midge.hh
)
set( midge_sources ${midge_sources}
	${core_dir}/coremsg.cc
	${core_dir}/stream.cc
	${core_dir}/input.cc
	${core_dir}/output.cc
	${core_dir}/node.cc
	${core_dir}/producer.cc
	${core_dir}/consumer.cc
	${core_dir}/transformer.cc
	${core_dir}/midge.cc
)



##################
# initialization #
##################

set( midge_headers ${midge_headers}
	${init_dir}/value.hh
	${init_dir}/object.hh
	${init_dir}/array.hh
	${init_dir}/lingual.hh
	${init_dir}/numerical.hh
	${init_dir}/boolean.hh
	${init_dir}/null.hh
	${init_dir}/processor.hh
	${init_dir}/arguments.hh
	${init_dir}/lexer.hh
	${init_dir}/reporter.hh
	${init_dir}/serializer.hh
	${init_dir}/evaluator.hh
	${init_dir}/compiler.hh
	${init_dir}/object_builder.hh
	${init_dir}/lingual_builder.hh
	${init_dir}/numerical_builder.hh
	${init_dir}/boolean_builder.hh
	${init_dir}/null_builder.hh
)
set( midge_sources ${midge_sources}
	${init_dir}/value.cc
	${init_dir}/object.cc
	${init_dir}/array.cc
	${init_dir}/lingual.cc
	${init_dir}/numerical.cc
	${init_dir}/boolean.cc
	${init_dir}/null.cc
	${init_dir}/processor.cc
	${init_dir}/arguments.cc
	${init_dir}/lexer.cc
	${init_dir}/reporter.cc
	${init_dir}/serializer.cc
	${init_dir}/compiler.cc
	${init_dir}/evaluator.cc
)


set( midge_test_initialization_directory
	test
)

set( midge_test_initialization_programs
	test_lexer_reporter
	test_lexer_serializer
	test_lexer_compiler_reporter
	test_lexer_compiler_serializer
	test_builder
)

midge_executables( test_initialization )

############
# bindings #
############

set( midge_headers ${midge_headers}
	${bind_dir}/midge_builder.hh
)
set( midge_sources ${midge_sources}
	${bind_dir}/midge_builder.cc
)


###########
# library #
###########

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/${bind_dir} )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/${core_dir} )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/${init_dir} )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/${util_dir} )

add_library( _midge SHARED ${midge_sources} )
target_link_libraries( _midge ${midge_external_libraries} )

install( FILES ${midge_headers} DESTINATION include )

install( TARGETS _midge DESTINATION lib )



########
# test #
########

set( midge_test_dependencies
	utility
	math
	core
	data
	producers
	consumers
	transformers
	initialization
)
set( midge_test_directory
	test
)
set( midge_test_programs
	test_harmonic_producer
	test_chirp_producer
	test_sinusoidal_producer
	test_gaussian_producer
	test_fourier_transformer
	test_analytic_transformer
	test_power_transformer
	test_wigner_transformer
)

midge_executables( test )

########
# json #
########

set( midge_json_directory
	test/json
)
set( midge_json_files
	test_types.json
	test_arrays.json
	test_objects.json
	test_builder.json
	test_comparison.json
)

midge_json( json )
