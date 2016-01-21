########
# main #
########

set( lib_dependencies 
    midge
)

set( programs
    midge
)

pbuilder_executables( programs lib_dependencies )
