# Minimum cmake verison 3.1 required for the variable CMAKE_CXX_STANDARD
cmake_minimum_required (VERSION 3.1)

#########
# setup #
#########

cmake_policy( SET CMP0048 NEW ) # version in project()
project( Midge VERSION 3.7.0 )

list( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/scarab/cmake )
include( PackageBuilder )

pbuilder_prepare_project()

list( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake )
include( MidgeUtility )


#################
# midge options #
#################

# require C++11
set_option( USE_CPP11 TRUE )

option( Midge_ENABLE_STREAM_TIMING "Enable stream timing" FALSE )

#option( MIDGE_ENABLE_DEBUG_MESSAGES "enable debug messages" FALSE )

midge_process_options()
  

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
    #${PROJECT_SOURCE_DIR}/library/bindings
    ${PROJECT_SOURCE_DIR}/library/core
    #${PROJECT_SOURCE_DIR}/library/initialization
    ${PROJECT_SOURCE_DIR}/library/utility
)

add_subdirectory( library )

add_subdirectory( test )


pbuilder_variables_for_parent()
