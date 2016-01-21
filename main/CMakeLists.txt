########
# main #
########

if( Midge_ENABLE_EXECUTABLES )

    set( lib_dependencies 
        Midge
    )
    
    set( programs
        midge
    )
    
    pbuilder_executables( programs lib_dependencies )

endif( Midge_ENABLE_EXECUTABLES )
