##########
# global #
##########

cmake_minimum_required( VERSION 2.8 )
project( midge C CXX )

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
#add_definitions( -std=c++11 )

############
# external #
############

# threads
find_package( Threads REQUIRED )

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
	${CMAKE_THREAD_LIBS_INIT}
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
	ascii.hh
	binary.hh
	plot.hh
	mutex.hh
	thread.hh
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
	ascii.cc
	binary.cc
	plot.cc
	mutex.cc
	thread.cc
)

midge_library( utility )

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
# math #
########

set( midge_math_dependencies
	utility
)
set( midge_math_directory
	math
)
set( midge_math_headers
	fourier.hh
	random.hh
	window.hh
	window_rectangular.hh
	window_bartlett.hh
	window_welch.hh
	window_hanning.hh
	window_hamming.hh
	window_blackman.hh
)
set( midge_math_sources
	fourier.cc
	random.cc
	window.cc
	window_rectangular.cc
	window_bartlett.cc
	window_welch.cc
	window_hanning.cc
	window_hamming.cc
	window_blackman.cc
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
	coremsg.hh
	stream.hh
	_stream.hh
	input.hh
	_input.hh
	output.hh
	_output.hh
	node.hh
	_buffer.hh
	_in.hh
	_out.hh
	producer.hh
	consumer.hh
	transformer.hh
	midge.hh
)
set( midge_core_sources
	coremsg.cc
	stream.cc
	input.cc
	output.cc
	node.cc
	producer.cc
	consumer.cc
	transformer.cc
	midge.cc
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
	_t_data.hh
	rt_data.hh
	ct_data.hh
	_f_data.hh
	rf_data.hh
	cf_data.hh
	_tf_data.hh
	rtf_data.hh
	ctf_data.hh
	point.hh
	point_data.hh
	line.hh
	line_data.hh
)
set( midge_data_sources
	rt_data.cc
	ct_data.cc
	rf_data.cc
	cf_data.cc
	rtf_data.cc
	ctf_data.cc
	point.cc
	point_data.cc
	line.cc
	line_data.cc
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
	_file_producer.hh
	rt_ascii_producer.hh
	ct_ascii_producer.hh
	rf_ascii_producer.hh
	cf_ascii_producer.hh
	rtf_ascii_producer.hh
	ctf_ascii_producer.hh
	rt_binary_producer.hh
	ct_binary_producer.hh
	rf_binary_producer.hh
	cf_binary_producer.hh
	rtf_binary_producer.hh
	ctf_binary_producer.hh
	rt_harmonic_producer.hh
	rt_chirp_producer.hh
	rt_sinusoidal_producer.hh
	rt_gaussian_producer.hh
	rt_monarch_producer.hh
)
set( midge_producers_sources
	rt_ascii_producer.cc
	ct_ascii_producer.cc
	rf_ascii_producer.cc
	cf_ascii_producer.cc
	rtf_ascii_producer.cc
	ctf_ascii_producer.cc
	rt_binary_producer.cc
	ct_binary_producer.cc
	rf_binary_producer.cc
	cf_binary_producer.cc
	rtf_binary_producer.cc
	ctf_binary_producer.cc
	rt_harmonic_producer.cc
	rt_chirp_producer.cc
	rt_sinusoidal_producer.cc
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
	_file_consumer.hh
	rt_ascii_consumer.hh
	ct_ascii_consumer.hh
	rf_ascii_consumer.hh
	cf_ascii_consumer.hh
	rtf_ascii_consumer.hh
	ctf_ascii_consumer.hh
	rt_binary_consumer.hh
	ct_binary_consumer.hh
	rf_binary_consumer.hh
	cf_binary_consumer.hh
	rtf_binary_consumer.hh
	ctf_binary_consumer.hh
	rt_monarch_consumer.hh
	rt_plot_consumer.hh
	rf_plot_consumer.hh
	rtf_plot_consumer.hh
	point_plot_consumer.hh
	line_plot_consumer.hh
)
set( midge_consumers_sources
	rt_ascii_consumer.cc
	ct_ascii_consumer.cc
	rf_ascii_consumer.cc
	cf_ascii_consumer.cc
	rtf_ascii_consumer.cc
	ctf_ascii_consumer.cc
	rt_binary_consumer.cc
	ct_binary_consumer.cc
	rf_binary_consumer.cc
	cf_binary_consumer.cc
	rtf_binary_consumer.cc
	ctf_binary_consumer.cc
	rt_monarch_consumer.cc
	rt_plot_consumer.cc
	rf_plot_consumer.cc
	rtf_plot_consumer.cc
	point_plot_consumer.cc
	line_plot_consumer.cc
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
	_converter_transformer.hh
	rt_ct_converter_transformer.hh
	ct_rt_converter_transformer.hh
	rf_cf_converter_transformer.hh
	cf_rf_converter_transformer.hh
	rtf_ctf_converter_transformer.hh
	ctf_rtf_converter_transformer.hh	
	_sum_transformer.hh
	rt_rt_sum_transformer.hh
	ct_ct_sum_transformer.hh
	rf_rf_sum_transformer.hh
	cf_cf_sum_transformer.hh
	rtf_rtf_sum_transformer.hh
	ctf_ctf_sum_transformer.hh
	_difference_transformer.hh
	rt_rt_difference_transformer.hh
	ct_ct_difference_transformer.hh
	rf_rf_difference_transformer.hh
	cf_cf_difference_transformer.hh
	rtf_rtf_difference_transformer.hh
	ctf_ctf_difference_transformer.hh
	_window_transformer.hh
	rt_rt_window_transformer.hh
	ct_ct_window_transformer.hh
	rf_rf_window_transformer.hh
	cf_cf_window_transformer.hh
	rtf_rtf_window_transformer.hh
	ctf_ctf_window_transformer.hh
	_smoothing_transformer.hh
	rt_rt_smoothing_transformer.hh
	ct_ct_smoothing_transformer.hh
	rf_rf_smoothing_transformer.hh
	cf_cf_smoothing_transformer.hh
	rtf_rtf_smoothing_transformer.hh
	ctf_ctf_smoothing_transformer.hh
	_average_transformer.hh
	rtf_rf_average_transformer.hh
	ctf_cf_average_transformer.hh
	ct_ctf_fourier_transformer.hh
	ctf_ct_fourier_transformer.hh	
	rt_ct_analytic_transformer.hh
	ctf_ctf_analytic_transformer.hh
	rt_rtf_power_transformer.hh
	rt_rtf_wigner_transformer.hh
	point_transformer.hh
	line_transformer.hh
)
set( midge_transformers_sources
	rt_ct_converter_transformer.cc
	ct_rt_converter_transformer.cc
	rf_cf_converter_transformer.cc
	cf_rf_converter_transformer.cc
	rtf_ctf_converter_transformer.cc
	ctf_rtf_converter_transformer.cc
	rt_rt_sum_transformer.cc
	ct_ct_sum_transformer.cc
	rf_rf_sum_transformer.cc
	cf_cf_sum_transformer.cc
	rtf_rtf_sum_transformer.cc
	ctf_ctf_sum_transformer.cc
	rt_rt_difference_transformer.cc
	ct_ct_difference_transformer.cc
	rf_rf_difference_transformer.cc
	cf_cf_difference_transformer.cc
	rtf_rtf_difference_transformer.cc
	ctf_ctf_difference_transformer.cc
	rt_rt_window_transformer.cc
	ct_ct_window_transformer.cc
	rf_rf_window_transformer.cc
	cf_cf_window_transformer.cc
	rtf_rtf_window_transformer.cc
	ctf_ctf_window_transformer.cc
	rt_rt_smoothing_transformer.cc
	ct_ct_smoothing_transformer.cc
	rf_rf_smoothing_transformer.cc
	cf_cf_smoothing_transformer.cc
	rtf_rtf_smoothing_transformer.cc
	ctf_ctf_smoothing_transformer.cc
	rtf_rf_average_transformer.cc
	ctf_cf_average_transformer.cc
	ct_ctf_fourier_transformer.cc
	ctf_ct_fourier_transformer.cc
	rt_ct_analytic_transformer.cc
	ctf_ctf_analytic_transformer.cc
	rt_rtf_power_transformer.cc
	rt_rtf_wigner_transformer.cc
	point_transformer.cc
	line_transformer.cc
)

midge_library( transformers )

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

set( midge_test_initialization_dependencies
	utility
	initialization
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

set( midge_bindings_dependencies
	utility
	math
	core
	data
	producers
	consumers
	transformers
	initialization
)
set( midge_bindings_directory
	bindings
)
set( midge_bindings_headers
	midge_builder.hh
	window_rectangular_builder.hh
	window_bartlett_builder.hh
	window_welch_builder.hh
	window_hanning_builder.hh
	window_hamming_builder.hh
	window_blackman_builder.hh
	rt_ascii_producer_builder.hh
	ct_ascii_producer_builder.hh
	rf_ascii_producer_builder.hh
	cf_ascii_producer_builder.hh
	rtf_ascii_producer_builder.hh
	ctf_ascii_producer_builder.hh
	rt_binary_producer_builder.hh
	ct_binary_producer_builder.hh
	rf_binary_producer_builder.hh
	cf_binary_producer_builder.hh
	rtf_binary_producer_builder.hh
	ctf_binary_producer_builder.hh
	rt_harmonic_producer_builder.hh
	rt_chirp_producer_builder.hh
	rt_sinusoidal_producer_builder.hh
	rt_gaussian_producer_builder.hh
	rt_monarch_producer_builder.hh
	rt_ascii_consumer_builder.hh
	ct_ascii_consumer_builder.hh
	rf_ascii_consumer_builder.hh
	cf_ascii_consumer_builder.hh
	rtf_ascii_consumer_builder.hh
	ctf_ascii_consumer_builder.hh
	rt_binary_consumer_builder.hh
	ct_binary_consumer_builder.hh
	rf_binary_consumer_builder.hh
	cf_binary_consumer_builder.hh
	rtf_binary_consumer_builder.hh
	ctf_binary_consumer_builder.hh
	rt_monarch_consumer_builder.hh
	rt_plot_consumer_builder.hh
	rf_plot_consumer_builder.hh
	rtf_plot_consumer_builder.hh
	point_plot_consumer_builder.hh
	line_plot_consumer_builder.hh
	rt_ct_converter_transformer_builder.hh
	ct_rt_converter_transformer_builder.hh
	rf_cf_converter_transformer_builder.hh
	cf_rf_converter_transformer_builder.hh
	rtf_ctf_converter_transformer_builder.hh
	ctf_rtf_converter_transformer_builder.hh
	rt_rt_sum_transformer_builder.hh
	ct_ct_sum_transformer_builder.hh
	rf_rf_sum_transformer_builder.hh
	cf_cf_sum_transformer_builder.hh
	rtf_rtf_sum_transformer_builder.hh
	ctf_ctf_sum_transformer_builder.hh
	rt_rt_difference_transformer_builder.hh
	ct_ct_difference_transformer_builder.hh
	rf_rf_difference_transformer_builder.hh
	cf_cf_difference_transformer_builder.hh
	rtf_rtf_difference_transformer_builder.hh
	ctf_ctf_difference_transformer_builder.hh
	rt_rt_window_transformer_builder.hh
	ct_ct_window_transformer_builder.hh
	rf_rf_window_transformer_builder.hh
	cf_cf_window_transformer_builder.hh
	rtf_rtf_window_transformer_builder.hh
	ctf_ctf_window_transformer_builder.hh
	rt_rt_smoothing_transformer_builder.hh
	ct_ct_smoothing_transformer_builder.hh
	rf_rf_smoothing_transformer_builder.hh
	cf_cf_smoothing_transformer_builder.hh
	rtf_rtf_smoothing_transformer_builder.hh
	ctf_ctf_smoothing_transformer_builder.hh
	rtf_rf_average_transformer_builder.hh
	ctf_cf_average_transformer_builder.hh
	ct_ctf_fourier_transformer_builder.hh
	ctf_ct_fourier_transformer_builder.hh
	rt_ct_analytic_transformer_builder.hh
	ctf_ctf_analytic_transformer_builder.hh
	rt_rtf_power_transformer_builder.hh
	rt_rtf_wigner_transformer_builder.hh
	point_transformer_builder.hh
	line_transformer_builder.hh
)
set( midge_bindings_sources
	midge_builder.cc
	rt_ascii_producer_builder.cc
	ct_ascii_producer_builder.cc
	rf_ascii_producer_builder.cc
	cf_ascii_producer_builder.cc
	rtf_ascii_producer_builder.cc
	ctf_ascii_producer_builder.cc
	rt_binary_producer_builder.cc
	ct_binary_producer_builder.cc
	rf_binary_producer_builder.cc
	cf_binary_producer_builder.cc
	rtf_binary_producer_builder.cc
	ctf_binary_producer_builder.cc
	rt_harmonic_producer_builder.cc
	rt_chirp_producer_builder.cc
	rt_sinusoidal_producer_builder.cc
	rt_gaussian_producer_builder.cc
	rt_monarch_producer_builder.cc
	rt_ascii_consumer_builder.cc
	ct_ascii_consumer_builder.cc
	rf_ascii_consumer_builder.cc
	cf_ascii_consumer_builder.cc
	rtf_ascii_consumer_builder.cc
	ctf_ascii_consumer_builder.cc
	rt_binary_consumer_builder.cc
	ct_binary_consumer_builder.cc
	rf_binary_consumer_builder.cc
	cf_binary_consumer_builder.cc
	rtf_binary_consumer_builder.cc
	ctf_binary_consumer_builder.cc
	rt_monarch_consumer_builder.cc
	rt_plot_consumer_builder.cc
	rf_plot_consumer_builder.cc
	rtf_plot_consumer_builder.cc
	point_plot_consumer_builder.cc
	line_plot_consumer_builder.cc
	rt_ct_converter_transformer_builder.cc
	ct_rt_converter_transformer_builder.cc
	rf_cf_converter_transformer_builder.cc
	cf_rf_converter_transformer_builder.cc
	rtf_ctf_converter_transformer_builder.cc
	ctf_rtf_converter_transformer_builder.cc
	rt_rt_sum_transformer_builder.cc
	ct_ct_sum_transformer_builder.cc
	rf_rf_sum_transformer_builder.cc
	cf_cf_sum_transformer_builder.cc
	rtf_rtf_sum_transformer_builder.cc
	ctf_ctf_sum_transformer_builder.cc
	rt_rt_difference_transformer_builder.cc
	ct_ct_difference_transformer_builder.cc
	rf_rf_difference_transformer_builder.cc
	cf_cf_difference_transformer_builder.cc
	rtf_rtf_difference_transformer_builder.cc
	ctf_ctf_difference_transformer_builder.cc
	rt_rt_window_transformer_builder.cc
	ct_ct_window_transformer_builder.cc
	rf_rf_window_transformer_builder.cc
	cf_cf_window_transformer_builder.cc
	rtf_rtf_window_transformer_builder.cc
	ctf_ctf_window_transformer_builder.cc
	rt_rt_smoothing_transformer_builder.cc
	ct_ct_smoothing_transformer_builder.cc
	rf_rf_smoothing_transformer_builder.cc
	cf_cf_smoothing_transformer_builder.cc
	rtf_rtf_smoothing_transformer_builder.cc
	ctf_ctf_smoothing_transformer_builder.cc
	rtf_rf_average_transformer_builder.cc
	ctf_cf_average_transformer_builder.cc
	ct_ctf_fourier_transformer_builder.cc
	ctf_ct_fourier_transformer_builder.cc
	rt_ct_analytic_transformer_builder.cc
	ctf_ctf_analytic_transformer_builder.cc
	rt_rtf_power_transformer_builder.cc
	rt_rtf_wigner_transformer_builder.cc
	point_transformer_builder.cc
	line_transformer_builder.cc
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
	json
)
set( midge_json_files
	test_types.json
	test_arrays.json
	test_objects.json
	test_builder.json
	test_fourier_transformer.json
	test_analytic_transformer.json
	test_power_transformer.json
	test_wigner_transformer.json
	test_harmonic_producer.json
	test_chirp_producer.json
	test_gaussian_producer.json
	test_monarch.json
	test_comparison.json
	generate_simulation_background.json
	generate_simulation_signal.json
	analyze_simulation_background.json
	analyze_simulation_signal.json
	reconstruct_simulation.json
	analyze_background.json
	analyze_signal.json
	reconstruct.json
)

midge_json( json )
