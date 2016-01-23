# Minimum cmake verison 3.0 required by the way CMake handles rpath information on Mac OSX.
cmake_minimum_required( VERSION 3.0 )

#########
# setup #
#########

cmake_policy( SET CMP0048 NEW ) # version in project()
project( Midge VERSION 3.3.1 )

list( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/scarab/cmake )

include( PackageBuilder )

pbuilder_prepare_project()


#################
# midge options #
#################

# require C++11
set_option( USE_CPP11 TRUE )


option( MIDGE_ENABLE_DEBUG_MESSAGES "enable debug messages" FALSE )
if( NOT CMAKE_BUILD_TYPE STREQUAL "" )
    string( TOLOWER ${CMAKE_BUILD_TYPE} LC_BUILD_TYPE )
endif( NOT CMAKE_BUILD_TYPE STREQUAL "" )
if( LC_BUILD_TYPE STREQUAL "debug" )
    set( MIDGE_ENABLE_DEBUG_MESSAGES FALSE CACHE INTERNAL "" FORCE )
    add_definitions( -DMIDGE_ENABLE_DEBUG_MESSAGES )    
else( LC_BUILD_TYPE STREQUAL "debug" )
    set( MIDGE_ENABLE_DEBUG_MESSAGES TRUE CACHE INTERNAL "" FORCE )
    remove_definitions( -DMIDGE_ENABLE_DEBUG_MESSAGES )
endif( LC_BUILD_TYPE STREQUAL "debug" )

  

######################
# midge dependencies #
######################

########
# Boost
########

# Boost (1.46 required for filesystem version 3)
#find_package( Boost 1.46.0 REQUIRED COMPONENTS filesystem date_time )
# make sure dynamic linking is assumed for all boost libraries
#add_definitions( -DBOOST_ALL_DYN_LINK )
#include_directories( ${Boost_INCLUDE_DIRS} )
#pbuilder_add_ext_libraries( ${Boost_LIBRARIES} )


########
# Scarab
########

pbuilder_add_submodule( Scarab scarab/library )


###############
# Build Midge #
###############

include_directories( BEFORE 
    ${PROJECT_SOURCE_DIR}/library/bindings
    ${PROJECT_SOURCE_DIR}/library/core
    ${PROJECT_SOURCE_DIR}/library/initialization
    ${PROJECT_SOURCE_DIR}/library/utility
)

add_subdirectory( library )

add_subdirectory( test )


pbuilder_variables_for_parent()
