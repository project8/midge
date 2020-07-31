cmake_minimum_required (VERSION 3.1)

cmake_policy( SET CMP0048 NEW ) # version in project()
project( MidgeDependent VERSION 1.0.0 )

### 
if( NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/midge )
    configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/copy_midge.sh.in ${CMAKE_CURRENT_BINARY_DIR}/copy_midge.sh @ONLY )
    execute_process( COMMAND ${CMAKE_CURRENT_BINARY_DIR}/copy_midge.sh
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} )
    if( EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/scarab )
        message( STATUS "Created submodule `midge`" )
    else()
        message( FATAL "Did not create submodule" )
    endif()
endif()
###

list( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/midge/scarab/cmake )

include( PackageBuilder )

pbuilder_prepare_project()

message( STATUS "Full project name for <${PROJECT_NAME}: ${${PROJECT_NAME}_FULL_PROJECT_NAME}" )

list( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/midge/cmake )
include( MidgeUtility )


#########
# Options
#########

midge_process_options()

##############
# Dependencies
##############


########
# Scarab
########

pbuilder_add_submodule( Midge midge )


#########
# Library
#########

pbuilder_use_sm_library( Midge Midge )

include_directories( BEFORE library )

set( MidgeDependent_HEADERS
    library/print.hh
)

set( MidgeDependent_SOURCES 
    library/print.cc
)

pbuilder_library( 
    TARGET MidgeDependent 
    SOURCES ${MidgeDependent_SOURCES}
)

pbuilder_component_install_and_export( 
    COMPONENT Library
    LIBTARGETS MidgeDependent
)

pbuilder_install_headers( ${MidgeDependent_HEADERS} )

midge_use_libraries( MidgeDependent )


############
# Executable
############

# Relies on Midge's executable
midge_build_executables()


########
# Config
########

configure_file( ${PROJECT_SOURCE_DIR}/PBTestConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/PBTestConfig.cmake @ONLY )

pbuilder_do_package_config()
