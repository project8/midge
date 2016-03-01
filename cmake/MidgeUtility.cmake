# Include the build script
# Main directory for midge

set( Midge_EXE_LIBRARIES )
set( Midge_LIB_FULL_NAME )

macro( midge_process_options )

if( Midge_ENABLE_STREAM_TIMING )
    add_definitions( -DENABLE_STREAM_TIMING )
else( Midge_ENABLE_STREAM_TIMING )
    remove_definitions( -DENABLE_STREAM_TIMING )
endif( Midge_ENABLE_STREAM_TIMING )

#if( NOT CMAKE_BUILD_TYPE STREQUAL "" )
#    string( TOLOWER ${CMAKE_BUILD_TYPE} LC_BUILD_TYPE )
#endif( NOT CMAKE_BUILD_TYPE STREQUAL "" )
#if( LC_BUILD_TYPE STREQUAL "debug" )
#    set( MIDGE_ENABLE_DEBUG_MESSAGES FALSE CACHE INTERNAL "" FORCE )
#    add_definitions( -DMIDGE_ENABLE_DEBUG_MESSAGES )    
#else( LC_BUILD_TYPE STREQUAL "debug" )
#    set( MIDGE_ENABLE_DEBUG_MESSAGES TRUE CACHE INTERNAL "" FORCE )
#    remove_definitions( -DMIDGE_ENABLE_DEBUG_MESSAGES )
#endif( LC_BUILD_TYPE STREQUAL "debug" )

endmacro( midge_process_options )

macro( midge_build_executables )
    #message(STATUS "midge source dir (from build_executables called from ${CMAKE_CURRENT_LIST_DIR}): ${Midge_SOURCE_DIR}")
    set( Midge_EXE_LIBRARIES ${Midge_EXE_LIBRARIES} ${Midge_LIBRARIES} )
    add_subdirectory( ${Midge_SOURCE_DIR}/main )
endmacro( midge_build_executables )

macro( midge_library LIB_BASENAME SOURCES PROJECT_LIBRARIES )
    pbuilder_library( ${LIB_BASENAME} ${SOURCES} ${PROJECT_LIBRARIES} )
    set( Midge_EXE_LIBRARIES ${Midge_EXE_LIBRARIES} ${FULL_LIB_NAME} PARENT_SCOPE )
endmacro( midge_library LIB_BASENAME SOURCES PROJECT_LIBRARIES )
