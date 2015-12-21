# module settings
set( MODULE_VERSION_MAJOR 0 )
set( MODULE_VERSION_MINOR 1 )
set( MODULE_VERSION_PATCH 0 )
set( MODULE_VERSION "${MODULE_VERSION_MAJOR}.${MODULE_VERSION_MINOR}.${MODULE_VERSION_PATCH}" )
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/" )

# rpath settings
if( APPLE )
    set( CMAKE_MACOSX_RPATH TRUE )
endif()
set( CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib" )
set( CMAKE_SKIP_RPATH FALSE )
set( CMAKE_SKIP_BUILD_RPATH  FALSE )
set( CMAKE_BUILD_WITH_INSTALL_RPATH TRUE )



macro( midge_prepare_project )


    #########
    # flags #
    #########
    
    set( MIDGE_ENABLE_DEBUG_MESSAGES "false" CACHE BOOL "enable debug messages" )
    if( MIDGE_ENABLE_DEBUG_MESSAGES )
        add_definitions( -DMIDGE_ENABLE_DEBUG_MESSAGES )
    endif()
    
    add_definitions( -Wall )
    add_definitions( -std=c++11 )
    
    ############
    # external #
    ############
    
    # threads
    find_package( Threads REQUIRED )
    
    # midge
    
    set( midge_external_libraries 
        ${CMAKE_THREAD_LIBS_INIT}
    )
    
    link_directories( ${midge_external_directories} )
    include_directories( ${midge_external_includes} )


endmacro( prepare_midge )



macro( midge_build_library )
endmacro( midge_build_library )

macro( midge_build_executables )
endmacro( midge_build_executables )


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
		target_link_libraries( ${program} _midge ${midge_${name}_dependency_names} ${midge_external_libraries} )
		install( TARGETS ${program} DESTINATION bin )
	endforeach( program )
	
endmacro( midge_executables )

####################################
# macro to define and install json #
####################################

macro( midge_json name )

	foreach( base ${midge_${name}_files} )
		list( APPEND midge_${name}_file_files ${CMAKE_CURRENT_SOURCE_DIR}/${midge_${name}_directory}/${base} )
	endforeach( base )
	
	install( FILES ${midge_${name}_file_files} DESTINATION json )
	
endmacro( midge_json )

