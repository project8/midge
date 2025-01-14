# Minimum cmake verison 3.1 required for the variable CMAKE_CXX_STANDARD
cmake_minimum_required (VERSION 3.1)

#########
# setup #
#########

cmake_policy( SET CMP0048 NEW ) # version in project()
project( Midge VERSION 3.9.3 )

list( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/scarab/cmake )
include( PackageBuilder )

list( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake )
include( MidgeUtility )


#################
# midge options #
#################

# require C++17
set_to_max( CMAKE_CXX_STANDARD 17 )

option( Midge_ENABLE_STREAM_TIMING "Enable stream timing" FALSE )

#option( MIDGE_ENABLE_DEBUG_MESSAGES "enable debug messages" FALSE )

midge_process_options()
  

######################
# midge dependencies #
######################

#####################
# Prepare for build #
#####################

pbuilder_prepare_project()


########
# Scarab
########

pbuilder_add_submodule( Scarab scarab )

# all parts of midge use Scarab, so include it here
pbuilder_use_sm_library( Scarab Scarab )


###############
# Build Midge #
###############

add_subdirectory( library )

if( Midge_ENABLE_TESTING )
    add_subdirectory( test )
endif( Midge_ENABLE_TESTING )


##########
# Config #
##########

configure_file( ${PROJECT_SOURCE_DIR}/MidgeConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/MidgeConfig.cmake @ONLY )

pbuilder_do_package_config()
