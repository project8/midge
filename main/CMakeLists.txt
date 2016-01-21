########
# main #
########

if( Midge_ENABLE_EXECUTABLES )

    set( lib_dependencies 
        ${Midge_EXE_LIBRARIES}
    )
    message(STATUS "midge lib dep: ${lib_dependencies}" )
    
    set( programs
        midge
    )
    
    pbuilder_executables( programs lib_dependencies )

endif( Midge_ENABLE_EXECUTABLES )
