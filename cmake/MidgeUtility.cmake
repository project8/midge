# Include the build script
include( ${CMAKE_CURRENT_LIST_DIR}/PackageBuilder.cmake )

# Main directory for midge
set( midge_dir ${CMAKE_CURRENT_LIST_DIR}/.. )

# module settings
set( MODULE_VERSION_MAJOR 0 )
set( MODULE_VERSION_MINOR 1 )
set( MODULE_VERSION_PATCH 0 )
set( MODULE_VERSION "${MODULE_VERSION_MAJOR}.${MODULE_VERSION_MINOR}.${MODULE_VERSION_PATCH}" )

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/" )




macro( midge_prepare_project )

    # custom midge name
    set( midge_project_name midge_${PROJECT_NAME} )

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
    
    #link_directories( BEFORE ${midge_external_directories} ) # currently there are no external directories
    #include_directories( BEFORE ${midge_external_includes} ) # currently there are no external includes
    
    
    ############
    # internal #
    ############
    
    include_directories( BEFORE 
        ${midge_dir}/library/bindings
        ${midge_dir}/library/core
        ${midge_dir}/library/initialization
        ${midge_dir}/library/utility
    )
    
    link_directories(
        ${midge_dir}/library
    )

endmacro( midge_prepare_project )



macro( midge_build_library )
    add_subdirectory( ${midge_dir}/library )
endmacro( midge_build_library )

macro( midge_build_executables )
    add_subdirectory( ${midge_dir}/main )
    add_subdirectory( ${midge_dir}/test )
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

	foreach( program ${midge_${name}_programs} )
		add_executable( ${program} ${CMAKE_CURRENT_SOURCE_DIR}/${program}.cc )
		target_link_libraries( ${program} ${midge_library} ${midge_${name}_dependencies} ${midge_external_libraries} )
		pbuilder_install_executables( ${program} )
	endforeach( program )
	
endmacro( midge_executables )



