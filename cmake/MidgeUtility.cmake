# Include the build script
# Main directory for midge

set( Midge_EXE_LIBRARIES )
set( Midge_LIB_FULL_NAME )

macro( midge_build_executables )
    #message(STATUS "midge source dir (from build_executables called from ${CMAKE_CURRENT_LIST_DIR}): ${Midge_SOURCE_DIR}")
    set( Midge_EXE_LIBRARIES ${Midge_EXE_LIBRARIES} ${Midge_LIBRARIES} )
    add_subdirectory( ${Midge_SOURCE_DIR}/main )
endmacro( midge_build_executables )

macro( midge_library LIB_BASENAME SOURCES PROJECT_LIBRARIES )
    pbuilder_library( ${LIB_BASENAME} ${SOURCES} ${PROJECT_LIBRARIES} )
    set( Midge_EXE_LIBRARIES ${Midge_EXE_LIBRARIES} ${FULL_LIB_NAME} PARENT_SCOPE )
endmacro( midge_library LIB_BASENAME SOURCES PROJECT_LIBRARIES )
