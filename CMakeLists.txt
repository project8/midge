cmake_minimum_required( VERSION 2.8 )
project( midge )

# module settings
set( MODULE_VERSION_MAJOR 0 )
set( MODULE_VERSION_MINOR 1 )
set( MODULE_VERSION_PATCH 0 )
set( MODULE_VERSION "${MODULE_VERSION_MAJOR}.${MODULE_VERSION_MINOR}.${MODULE_VERSION_PATCH}" )
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/" )

# rpath shit
set( CMAKE_MACOSX_RPATH TRUE )
set( CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib" )
set( CMAKE_SKIP_RPATH FALSE )
set( CMAKE_SKIP_BUILD_RPATH  FALSE )
set( CMAKE_BUILD_WITH_INSTALL_RPATH TRUE ) 

############
# external #
############

# fftw
find_package( FFTW REQUIRED )

# gsl
find_package( GSL REQUIRED )

set( midge_external_includes{
	${FFTW_INCLUDE_DIR}
	${GSL_INCLUDE_DIRS}
)

set( midge_external_libraries 
	${FFTW_LIBRARIES}
	${GSL_LIBRARIES}
)

include_directories( ${midge_external_includes} )


###########
# utility #
###########

set( midge_utility_name
	utility
)
set( midge_utility_dependencies
)
set( midge_utility_directory
	utility
)
set( midge_utility_headers
	error.hh
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
)

foreach( base ${midge_utility_headers} )
	list( APPEND midge_utility_header_files ${CMAKE_CURRENT_SOURCE_DIR}/${midge_utility_directory}/${base} )
endforeach( base )

foreach( base ${midge_utility_sources} )
	list( APPEND midge_utility_source_files ${CMAKE_CURRENT_SOURCE_DIR}/${midge_utility_directory}/${base} )
endforeach( base )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/${midge_utility_directory} )
add_library( ${midge_utility_name} SHARED ${midge_utility_header_files} ${midge_utility_source_files} )
target_link_libraries( ${midge_utility_name} ${midge_utility_dependencies} ${midge_external_libraries} )
install( FILES ${midge_utility_header_files} DESTINATION include )
install( TARGETS ${midge_utility_name} DESTINATION lib )

########
# core #
########

set( midge_core_name
	core
)
set( midge_core_dependencies
	utility
)
set( midge_core_directory
	core
)
set( midge_core_headers
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
)
set( midge_core_sources
	root.cc
	link.cc
	node.cc
	data.cc
	producer.cc
	consumer.cc
	transformer.cc
	controller.cc
)

foreach( base ${midge_core_headers} )
	list( APPEND midge_core_header_files ${CMAKE_CURRENT_SOURCE_DIR}/${midge_core_directory}/${base} )
endforeach( base )

foreach( base ${midge_core_sources} )
	list( APPEND midge_core_source_files ${CMAKE_CURRENT_SOURCE_DIR}/${midge_core_directory}/${base} )
endforeach( base )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/${midge_core_directory} )
add_library( ${midge_core_name} SHARED ${midge_core_header_files} ${midge_core_source_files} )
target_link_libraries( ${midge_core_name} ${midge_core_dependencies} ${midge_external_libraries} )
install( FILES ${midge_core_header_files} DESTINATION include )
install( TARGETS ${midge_core_name} DESTINATION lib )

########
# data #
########

set( midge_data_name
	data
)
set( midge_data_dependencies
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

foreach( base ${midge_data_headers} )
	list( APPEND midge_data_header_files ${CMAKE_CURRENT_SOURCE_DIR}/${midge_data_directory}/${base} )
endforeach( base )

foreach( base ${midge_data_sources} )
	list( APPEND midge_data_source_files ${CMAKE_CURRENT_SOURCE_DIR}/${midge_data_directory}/${base} )
endforeach( base )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/${midge_data_directory} )
add_library( ${midge_data_name} SHARED ${midge_data_header_files} ${midge_data_source_files} )
target_link_libraries( ${midge_data_name} ${midge_data_dependencies} ${midge_external_libraries} )
install( FILES ${midge_data_header_files} DESTINATION include )
install( TARGETS ${midge_data_name} DESTINATION lib )

#############
# producers #
#############

set( midge_producers_name
	producers
)
set( midge_producers_dependencies
	core
	data
)
set( midge_producers_directory
	producers
)
set( midge_producers_headers
	rt_harmonic_producer.hh
	rt_gaussian_producer.hh
)
set( midge_producers_sources
	rt_harmonic_producer.cc
	rt_gaussian_producer.cc
)

foreach( base ${midge_producers_headers} )
	list( APPEND midge_producers_header_files ${CMAKE_CURRENT_SOURCE_DIR}/${midge_producers_directory}/${base} )
endforeach( base )

foreach( base ${midge_producers_sources} )
	list( APPEND midge_producers_source_files ${CMAKE_CURRENT_SOURCE_DIR}/${midge_producers_directory}/${base} )
endforeach( base )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/${midge_producers_directory} )
add_library( ${midge_producers_name} SHARED ${midge_producers_header_files} ${midge_producers_source_files} )
target_link_libraries( ${midge_producers_name} ${midge_producers_dependencies} ${midge_external_libraries} )
install( FILES ${midge_producers_header_files} DESTINATION include )
install( TARGETS ${midge_producers_name} DESTINATION lib )

#############
# consumers #
#############

set( midge_consumers_name
	consumers
)
set( midge_consumers_dependencies
	core
	data
)
set( midge_consumers_directory
	consumers
)
set( midge_consumers_headers
	rt_ascii_consumer.hh
	rf_ascii_consumer.hh
)
set( midge_consumers_sources
	rt_ascii_consumer.cc
	rf_ascii_consumer.cc
)

foreach( base ${midge_consumers_headers} )
	list( APPEND midge_consumers_header_files ${CMAKE_CURRENT_SOURCE_DIR}/${midge_consumers_directory}/${base} )
endforeach( base )

foreach( base ${midge_consumers_sources} )
	list( APPEND midge_consumers_source_files ${CMAKE_CURRENT_SOURCE_DIR}/${midge_consumers_directory}/${base} )
endforeach( base )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/${midge_consumers_directory} )
add_library( ${midge_consumers_name} SHARED ${midge_consumers_header_files} ${midge_consumers_source_files} )
target_link_libraries( ${midge_consumers_name} ${midge_consumers_dependencies} ${midge_external_libraries} )
install( FILES ${midge_consumers_header_files} DESTINATION include )
install( TARGETS ${midge_consumers_name} DESTINATION lib )

################
# transformers #
################

set( midge_transformers_name
	transformers
)
set( midge_transformers_dependencies
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
	ct_cf_fourier_transformer.hh
	cf_ct_fourier_transformer.hh
	rt_rf_power_transformer.hh
)
set( midge_transformers_sources
	rt_ct_transformer.cc
	rf_cf_transformer.cc
	ct_rt_transformer.cc
	cf_rf_transformer.cc
	ct_cf_fourier_transformer.cc
	cf_ct_fourier_transformer.cc
	rt_rf_power_transformer.cc
)

foreach( base ${midge_transformers_headers} )
	list( APPEND midge_transformers_header_files ${CMAKE_CURRENT_SOURCE_DIR}/${midge_transformers_directory}/${base} )
endforeach( base )

foreach( base ${midge_transformers_sources} )
	list( APPEND midge_transformers_source_files ${CMAKE_CURRENT_SOURCE_DIR}/${midge_transformers_directory}/${base} )
endforeach( base )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/${midge_transformers_directory} )
add_library( ${midge_transformers_name} SHARED ${midge_transformers_header_files} ${midge_transformers_source_files} )
target_link_libraries( ${midge_transformers_name} ${midge_transformers_dependencies} ${midge_external_libraries} )
install( FILES ${midge_transformers_header_files} DESTINATION include )
install( TARGETS ${midge_transformers_name} DESTINATION lib )

################
# controllers #
################

set( midge_controllers_name
	controllers
)
set( midge_controllers_dependencies
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
	count_controller.hh
)
set( midge_controllers_sources
	count_controller.cc
)

foreach( base ${midge_controllers_headers} )
	list( APPEND midge_controllers_header_files ${CMAKE_CURRENT_SOURCE_DIR}/${midge_controllers_directory}/${base} )
endforeach( base )

foreach( base ${midge_controllers_sources} )
	list( APPEND midge_controllers_source_files ${CMAKE_CURRENT_SOURCE_DIR}/${midge_controllers_directory}/${base} )
endforeach( base )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/${midge_controllers_directory} )
add_library( ${midge_controllers_name} SHARED ${midge_controllers_header_files} ${midge_controllers_source_files} )
target_link_libraries( ${midge_controllers_name} ${midge_controllers_dependencies} ${midge_external_libraries} )
install( FILES ${midge_controllers_header_files} DESTINATION include )
install( TARGETS ${midge_controllers_name} DESTINATION lib )

##################
# initialization #
##################

set( midge_initialization_name
	initialization
)
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

foreach( base ${midge_initialization_headers} )
	list( APPEND midge_initialization_header_files ${CMAKE_CURRENT_SOURCE_DIR}/${midge_initialization_directory}/${base} )
endforeach( base )

foreach( base ${midge_initialization_sources} )
	list( APPEND midge_initialization_source_files ${CMAKE_CURRENT_SOURCE_DIR}/${midge_initialization_directory}/${base} )
endforeach( base )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/${midge_initialization_directory} )
add_library( ${midge_initialization_name} SHARED ${midge_initialization_header_files} ${midge_initialization_source_files} )
target_link_libraries( ${midge_initialization_name} ${midge_initialization_dependencies} ${midge_external_libraries} )
install( FILES ${midge_initialization_header_files} DESTINATION include )
install( TARGETS ${midge_initialization_name} DESTINATION lib )

############
# bindings #
############

set( midge_bindings_name
	bindings
)
set( midge_bindings_dependencies
	utility
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
	root_builder.hh
	rt_data_builder.hh
	rf_data_builder.hh
	ct_data_builder.hh
	cf_data_builder.hh
	rt_harmonic_producer_builder.hh
	rt_gaussian_producer_builder.hh
	rt_ascii_consumer_builder.hh
	rf_ascii_consumer_builder.hh
	rt_ct_transformer_builder.hh
	rf_cf_transformer_builder.hh
	ct_rt_transformer_builder.hh
	cf_rf_transformer_builder.hh
	ct_cf_fourier_transformer_builder.hh
	cf_ct_fourier_transformer_builder.hh
	rt_rf_power_transformer_builder.hh
)
set( midge_bindings_sources
	root_builder.cc
	rt_data_builder.cc
	rf_data_builder.cc
	ct_data_builder.cc
	cf_data_builder.cc
	rt_harmonic_producer_builder.cc
	rt_gaussian_producer_builder.cc
	rt_ascii_consumer_builder.cc
	rf_ascii_consumer_builder.cc
	rt_ct_transformer_builder.cc
	rf_cf_transformer_builder.cc
	ct_rt_transformer_builder.cc
	cf_rf_transformer_builder.cc
	ct_cf_fourier_transformer_builder.cc
	cf_ct_fourier_transformer_builder.cc
	rt_rf_power_transformer_builder.cc
)

foreach( base ${midge_bindings_headers} )
	list( APPEND midge_bindings_header_files ${CMAKE_CURRENT_SOURCE_DIR}/${midge_bindings_directory}/${base} )
endforeach( base )

foreach( base ${midge_bindings_sources} )
	list( APPEND midge_bindings_source_files ${CMAKE_CURRENT_SOURCE_DIR}/${midge_bindings_directory}/${base} )
endforeach( base )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/${midge_bindings_directory} )
add_library( ${midge_bindings_name} SHARED ${midge_bindings_header_files} ${midge_bindings_source_files} )
target_link_libraries( ${midge_bindings_name} ${midge_bindings_dependencies} ${midge_external_libraries} )
install( FILES ${midge_bindings_header_files} DESTINATION include )
install( TARGETS ${midge_bindings_name} DESTINATION lib )

########
# main #
########

set( midge_main_dependencies
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

foreach( program ${midge_main_programs} )
	add_executable( ${program} ${CMAKE_CURRENT_SOURCE_DIR}/${midge_main_directory}/${program}.cc )
	target_link_libraries( ${program} ${midge_main_dependencies} ${midge_external_libraries} )
	install( TARGETS ${program} DESTINATION bin )
endforeach( program )

########
# test #
########

set( midge_test_dependencies
	core
	data
	producers
	consumers
	transformers
	initialization
	bindings
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
	test_harmonic_producer
	test_fourier_transformer
)

foreach( program ${midge_test_programs} )
	add_executable( ${program} ${CMAKE_CURRENT_SOURCE_DIR}/${midge_test_directory}/${program}.cc )
	target_link_libraries( ${program} ${midge_test_dependencies} ${midge_external_libraries} )
	install( TARGETS ${program} DESTINATION bin )
endforeach( program )

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
	test_harmonic_producer.json
	test_gaussian_producer.json
	test_fourier_transformer.json
)

foreach( base ${midge_json_files} )
	list( APPEND midge_json_file_files ${CMAKE_CURRENT_SOURCE_DIR}/${midge_json_directory}/${base} )
endforeach( base )

install( FILES ${midge_json_file_files} DESTINATION config )
