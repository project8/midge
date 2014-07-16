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

set( midge_external_includes{
	${FFTW_INCLUDE_DIR}
)

set( midge_external_libraries 
	${FFTW_LIBRARIES}
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
	typenull.hh
	typenode.hh
	typelist.hh
	typelength.hh
	typeat.hh
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
	node.hh
	link.hh
	member.hh
	component.hh
	operand.hh
	in.hh
	out.hh
	transformer.hh
)
set( midge_core_sources
	node.cc
	link.cc
	component.cc
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

############
# operands #
############

set( midge_operands_name
	operands
)
set( midge_operands_dependencies
	core
)
set( midge_operands_directory
	operands
)
set( midge_operands_headers
	types.hh
	real.hh
	complex.hh
)
set( midge_operands_sources
	real.cc
	complex.cc
)

foreach( base ${midge_operands_headers} )
	list( APPEND midge_operands_header_files ${CMAKE_CURRENT_SOURCE_DIR}/${midge_operands_directory}/${base} )
endforeach( base )

foreach( base ${midge_operands_sources} )
	list( APPEND midge_operands_source_files ${CMAKE_CURRENT_SOURCE_DIR}/${midge_operands_directory}/${base} )
endforeach( base )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/${midge_operands_directory} )
add_library( ${midge_operands_name} SHARED ${midge_operands_header_files} ${midge_operands_source_files} )
target_link_libraries( ${midge_operands_name} ${midge_operands_dependencies} ${midge_external_libraries} )
install( FILES ${midge_operands_header_files} DESTINATION include )
install( TARGETS ${midge_operands_name} DESTINATION lib )

#############
# producers #
#############

set( midge_producers_name
	producers
)
set( midge_producers_dependencies
	core
	operands
)
set( midge_producers_directory
	producers
)
set( midge_producers_headers
	harmonic_real_producer.hh
)
set( midge_producers_sources
	harmonic_real_producer.cc
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

################
# transformers #
################

set( midge_transformers_name
	transformers
)
set( midge_transformers_dependencies
	core
	operands
)
set( midge_transformers_directory
	transformers
)
set( midge_transformers_headers
	complex_forward_fourier_transformer.hh
	complex_backward_fourier_transformer.hh
)
set( midge_transformers_sources
	complex_forward_fourier_transformer.cc
	complex_backward_fourier_transformer.cc
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

########
# test #
########

set( midge_test_dependencies
	core
	operands
	transformers
)
set( midge_test_directory
	test
)
set( midge_test_programs
	test_typelength
	test_typeat
	test_real
	test_complex
)

foreach( program ${midge_test_programs} )
	add_executable( ${program} ${CMAKE_CURRENT_SOURCE_DIR}/${midge_test_directory}/${program}.cc )
	target_link_libraries( ${program} ${midge_test_dependencies} ${midge_external_libraries} )
	install( TARGETS ${program} DESTINATION bin )
endforeach( program )

