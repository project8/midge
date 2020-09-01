########
# main #
########

message( "Building Midge main executable" )
message(STATUS "Midge external dependencies: ${Midge_EXE_LIBRARIES}" )

if( NOT Midge_AS_SUBMODULE )
    set( Midge_package_libs
        Midge
    )
endif()

set( Midge_sources
    midge.cc
)

set( Midge_programs )
pbuilder_executables( 
    SOURCES ${Midge_sources}
    TARGETS_VAR Midge_programs
    PROJECT_LIBRARIES ${Midge_package_libs}
    PUBLIC_EXTERNAL_LIBRARIES ${Midge_EXE_LIBRARIES}
#        PRIVATE_EXTERNAL_LIBRARIES ${PRIVATE_EXT_LIBS}
)

pbuilder_component_install_and_export( 
    COMPONENT Executables
    EXETARGETS ${Midge_programs}
)

