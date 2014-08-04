#########################################
# macro to define and install libraries #
#########################################

macro( midge_library name )

	foreach( dependency ${midge_${name}_dependencies} )
		list( APPEND midge_${name}_dependency_names _midge_${dependency} )
	endforeach( dependency )
	
	foreach( base ${midge_${name}_headers} )
		list( APPEND midge_${name}_header_files ${CMAKE_CURRENT_SOURCE_DIR}/${midge_${name}_directory}/${base} )
	endforeach( base )
	
	foreach( base ${midge_${name}_sources} )
		list( APPEND midge_${name}_source_files ${CMAKE_CURRENT_SOURCE_DIR}/${midge_${name}_directory}/${base} )
	endforeach( base )
	
	include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/${midge_${name}_directory} )
	
	add_library( _midge_${name} SHARED ${midge_${name}_header_files} ${midge_${name}_source_files} )
	target_link_libraries( _midge_${name} ${midge_${name}_dependency_names} ${midge_external_libraries} )
	
	install( FILES ${midge_${name}_header_files} DESTINATION include )
	
	install( TARGETS _midge_${name} DESTINATION lib )

endmacro( midge_library )

###########################################
# macro to define and install executables #
###########################################

macro( midge_executables name )

	foreach( dependency ${midge_${name}_dependencies} )
		list( APPEND midge_${name}_dependency_names _midge_${dependency} )
	endforeach( dependency )
	
	foreach( program ${midge_${name}_programs} )
		add_executable( ${program} ${CMAKE_CURRENT_SOURCE_DIR}/${midge_${name}_directory}/${program}.cc )
		target_link_libraries( ${program} ${midge_${name}_dependency_names} ${midge_external_libraries} )
		install( TARGETS ${program} DESTINATION bin )
	endforeach( program )
	
endmacro( midge_executables )

##############################################
# macro to define and install configurations #
##############################################

macro( midge_configurations name )

	foreach( base ${midge_${name}_files} )
		list( APPEND midge_${name}_file_files ${CMAKE_CURRENT_SOURCE_DIR}/${midge_${name}_directory}/${base} )
	endforeach( base )
	
	install( FILES ${midge_${name}_file_files} DESTINATION config )
	
endmacro( midge_configurations )

