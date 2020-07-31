########
# test #
########

set( lib_dependencies
    Midge
)

set( sources
    test_buffer.cc
    test_message.cc
    test_producer.cc
    test_signal.cc
    test_slot.cc
    test_typelength.cc
    test_typeat.cc
    test_typechain.cc
    test_typereplace.cc
)

set( programs )
pbuilder_executables( 
    SOURCES ${sources}
    TARGETS_VAR programs
    PROJECT_LIBRARIES ${lib_dependencies}
)

pbuilder_component_install_and_export( 
    COMPONENT Test
    EXETARGETS ${programs}
)
