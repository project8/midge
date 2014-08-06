##########
# global #
##########

cmake_minimum_required( VERSION 2.8 )
project( midge )

# module settings
set( MODULE_VERSION_MAJOR 0 )
set( MODULE_VERSION_MINOR 1 )
set( MODULE_VERSION_PATCH 0 )
set( MODULE_VERSION "${MODULE_VERSION_MAJOR}.${MODULE_VERSION_MINOR}.${MODULE_VERSION_PATCH}" )
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/" )

# rpath settings
set( CMAKE_MACOSX_RPATH TRUE )
set( CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib" )
set( CMAKE_SKIP_RPATH FALSE )
set( CMAKE_SKIP_BUILD_RPATH  FALSE )
set( CMAKE_BUILD_WITH_INSTALL_RPATH TRUE )

# utilities

include( MidgeUtility )

#########
# flags #
#########

set( MIDGE_ENABLE_DEBUG_MESSAGES "false" CACHE BOOL "enable debug messages" )
if( MIDGE_ENABLE_DEBUG_MESSAGES )
	add_definitions( -DMIDGE_ENABLE_DEBUG_MESSAGES )
endif()
add_definitions( -Wall )

############
# external #
############

# fftw
find_package( FFTW REQUIRED )

# gsl
find_package( GSL REQUIRED )

# root
find_package( ROOT 5.24.0 REQUIRED )

# monarch
set( Monarch_INSTALL_DIR "/usr/local" CACHE PATH "path to monarch installation" )
set( Monarch_INCLUDE_DIR ${Monarch_INSTALL_DIR}/include )
set( Monarch_LIBRARY_DIR ${Monarch_INSTALL_DIR}/lib )
set( Monarch_LIBRARIES -lMonarchCore -lMonarchProto )

# midge
set( midge_external_directories
	${GSL_LIBRARY_DIRS}
	${ROOT_LIBRARY_DIRS}
	${Monarch_LIBRARY_DIR}
)

set( midge_external_includes
	${FFTW_INCLUDE_DIR}
	${GSL_INCLUDE_DIRS}
	${ROOT_INCLUDE_DIR}
	${Monarch_INCLUDE_DIR}
)

set( midge_external_libraries 
	${FFTW_LIBRARIES}
	${GSL_LIBRARIES}
	${ROOT_LIBRARIES}
	${Monarch_LIBRARIES}
)

link_directories( ${midge_external_directories} )
include_directories( ${midge_external_includes} )

###########
# utility #
###########

set( midge_utility_dependencies
)
set( midge_utility_directory
	utility
)
set( midge_utility_headers
	macros.hh
	initializer.hh
	singleton.hh
	error.hh
	message.hh
	types.hh
	typenull.hh
	typenode.hh
	typelist.hh
	typelength.hh
	typeat.hh
	typein.hh
	typeif.hh
	typechain.hh
)
set( midge_utility_sources
	error.cc
	message.cc
)

midge_library( utility )

########
# math #
########

set( midge_math_dependencies
	utility
)
set( midge_math_directory
	math
)
set( midge_math_headers
	window.hh
	window_rectangular.hh
	window_bartlett.hh
	window_welch.hh
	window_hanning.hh
	window_hamming.hh
)
set( midge_math_sources
	window.cc
	window_rectangular.cc
	window_bartlett.cc
	window_welch.cc
	window_hanning.cc
	window_hamming.cc
)

midge_library( math )

########
# core #
########

set( midge_core_dependencies
	utility
	math
)
set( midge_core_directory
	core
)
set( midge_core_headers
	core_message.hh
	root.hh
	link.hh
	member.hh
	node.hh
	data.hh
	in.hh
	out.hh
	producer.hh
	consumer.hh
	transformer.hh
	controller.hh
	plot.hh
)
set( midge_core_sources
	core_message.cc
	root.cc
	link.cc
	node.cc
	data.cc
	producer.cc
	consumer.cc
	transformer.cc
	controller.cc
	plot.cc
)

midge_library( core )

########
# data #
########

set( midge_data_dependencies
	utility
	math
	core
)
set( midge_data_directory
	data
)
set( midge_data_headers
	rt_data.hh
	rf_data.hh
	ct_data.hh
	cf_data.hh
)
set( midge_data_sources
	rt_data.cc
	rf_data.cc
	ct_data.cc
	cf_data.cc
)

midge_library( data )

#############
# producers #
#############

set( midge_producers_dependencies
	utility
	math
	core
	data
)
set( midge_producers_directory
	producers
)
set( midge_producers_headers
	rt_harmonic_producer.hh
	rt_chirp_producer.hh
	rt_gaussian_producer.hh
	rt_monarch_producer.hh
)
set( midge_producers_sources
	rt_harmonic_producer.cc
	rt_chirp_producer.cc
	rt_gaussian_producer.cc
	rt_monarch_producer.cc
)

midge_library( producers )

#############
# consumers #
#############

set( midge_consumers_dependencies
	utility
	math
	core
	data
)
set( midge_consumers_directory
	consumers
)
set( midge_consumers_headers
	rt_ascii_consumer.hh
	rf_ascii_consumer.hh
	rt_root_consumer.hh
	rf_root_consumer.hh
	rt_monarch_consumer.hh
	rf_background_consumer.hh
	rf_candidate_consumer.hh
)
set( midge_consumers_sources
	rt_ascii_consumer.cc
	rf_ascii_consumer.cc
	rt_root_consumer.cc
	rf_root_consumer.cc
	rt_monarch_consumer.cc
	rf_background_consumer.cc
	rf_candidate_consumer.cc
)

midge_library( consumers )

################
# transformers #
################

set( midge_transformers_dependencies
	utility
	math
	core
	data
)
set( midge_transformers_directory
	transformers
)
set( midge_transformers_headers
	rt_ct_transformer.hh
	rf_cf_transformer.hh
	ct_rt_transformer.hh
	cf_rf_transformer.hh
	rt_rt_sum_transformer.hh
	ct_ct_sum_transformer.hh
	ct_cf_fourier_transformer.hh
	cf_ct_fourier_transformer.hh
	rt_ct_analytic_transformer.hh
	rt_rf_power_transformer.hh
	rt_rf_wigner_transformer.hh
	rf_rf_threshold_transformer.hh
)
set( midge_transformers_sources
	rt_ct_transformer.cc
	rf_cf_transformer.cc
	ct_rt_transformer.cc
	cf_rf_transformer.cc
	rt_rt_sum_transformer.cc
	ct_ct_sum_transformer.cc
	ct_cf_fourier_transformer.cc
	cf_ct_fourier_transformer.cc
	rt_ct_analytic_transformer.cc
	rt_rf_power_transformer.cc
	rt_rf_wigner_transformer.cc
	rf_rf_threshold_transformer.cc
)

midge_library( transformers )

################
# controllers #
################

set( midge_controllers_dependencies
	utility
	math
	core
	data
	producers
	consumers
	transformers
)
set( midge_controllers_directory
	controllers
)
set( midge_controllers_headers
	void_controller.hh
	count_controller.hh
)
set( midge_controllers_sources
	void_controller.cc
	count_controller.cc
)

midge_library( controllers )

##################
# initialization #
##################

set( midge_initialization_dependencies
	utility
)
set( midge_initialization_directory
	initialization
)
set( midge_initialization_headers
	value.hh
	object.hh
	array.hh
	lingual.hh
	numerical.hh
	boolean.hh
	null.hh
	processor.hh
	lexer.hh
	reporter.hh
	serializer.hh
	compiler.hh
	object_builder.hh
	lingual_builder.hh
	numerical_builder.hh
	boolean_builder.hh
	null_builder.hh
)
set( midge_initialization_sources
	value.cc
	object.cc
	array.cc
	lingual.cc
	numerical.cc
	boolean.cc
	null.cc
	processor.cc
	lexer.cc
	reporter.cc
	serializer.cc
	compiler.cc
)

midge_library( initialization )

############
# bindings #
############

set( midge_bindings_dependencies
	utility
	math
	core
	data
	producers
	consumers
	transformers
	controllers
	initialization
)
set( midge_bindings_directory
	bindings
)
set( midge_bindings_headers
	root_builder.hh
	window_rectangular_builder.hh
	window_bartlett_builder.hh
	window_welch_builder.hh
	window_hanning_builder.hh
	window_hamming_builder.hh
	rt_data_builder.hh
	rf_data_builder.hh
	ct_data_builder.hh
	cf_data_builder.hh
	rt_harmonic_producer_builder.hh
	rt_chirp_producer_builder.hh
	rt_gaussian_producer_builder.hh
	rt_monarch_producer_builder.hh
	rt_ascii_consumer_builder.hh
	rf_ascii_consumer_builder.hh
	rt_root_consumer_builder.hh
	rf_root_consumer_builder.hh
	rt_monarch_consumer_builder.hh
	rf_background_consumer_builder.hh
	rf_candidate_consumer_builder.hh
	rt_ct_transformer_builder.hh
	rf_cf_transformer_builder.hh
	ct_rt_transformer_builder.hh
	cf_rf_transformer_builder.hh
	rt_rt_sum_transformer_builder.hh
	ct_ct_sum_transformer_builder.hh
	ct_cf_fourier_transformer_builder.hh
	cf_ct_fourier_transformer_builder.hh
	rt_ct_analytic_transformer_builder.hh
	rt_rf_power_transformer_builder.hh
	rt_rf_wigner_transformer_builder.hh
	rf_rf_threshold_transformer_builder.hh
	void_controller_builder.hh
	count_controller_builder.hh
)
set( midge_bindings_sources
	root_builder.cc
	rt_data_builder.cc
	rf_data_builder.cc
	ct_data_builder.cc
	cf_data_builder.cc
	rt_harmonic_producer_builder.cc
	rt_chirp_producer_builder.cc
	rt_gaussian_producer_builder.cc
	rt_monarch_producer_builder.cc
	rt_ascii_consumer_builder.cc
	rf_ascii_consumer_builder.cc
	rt_root_consumer_builder.cc
	rf_root_consumer_builder.cc
	rt_monarch_consumer_builder.cc
	rf_background_consumer_builder.cc
	rf_candidate_consumer_builder.cc
	rt_ct_transformer_builder.cc
	rf_cf_transformer_builder.cc
	ct_rt_transformer_builder.cc
	cf_rf_transformer_builder.cc
	rt_rt_sum_transformer_builder.cc
	ct_ct_sum_transformer_builder.cc
	ct_cf_fourier_transformer_builder.cc
	cf_ct_fourier_transformer_builder.cc
	rt_ct_analytic_transformer_builder.cc
	rt_rf_power_transformer_builder.cc
	rt_rf_wigner_transformer_builder.cc
	rf_rf_threshold_transformer_builder.cc
	void_controller_builder.cc
	count_controller_builder.cc
)

midge_library( bindings )

########
# main #
########

set( midge_main_dependencies
	utility
	math
	core
	data
	producers
	consumers
	transformers
	controllers
	initialization
	bindings
)
set( midge_main_directory
	main
)
set( midge_main_programs
	midge
)

midge_executables( main )

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
	controllers
	initialization
)
set( midge_test_directory
	test
)
set( midge_test_programs
	test_lexer_reporter
	test_lexer_serializer
	test_lexer_compiler_reporter
	test_lexer_compiler_serializer
	test_builder
	test_typelength
	test_typeat
	test_real
	test_complex
	test_fourier_transformer
	test_analytic_transformer
	test_wigner_transformer
	test_harmonic_producer
	test_chirp_producer
	test_gaussian_producer
)

midge_executables( test )

########
# json #
########

set( midge_json_directory
	json
)
set( midge_json_files
	test_types.json
	test_arrays.json
	test_objects.json
	test_builder.json
	test_fourier_transformer.json
	test_analytic_transformer.json
	test_harmonic_producer.json
	test_chirp_producer.json
	test_gaussian_producer.json
	test_comparison.json
	test_monarch.json
	test_analysis.json
)

midge_configurations( json )
