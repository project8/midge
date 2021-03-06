# CMakeLists.txt for midge/library

set( core_dir core )
set( util_dir utility )

include_directories( BEFORE 
    ${PROJECT_SOURCE_DIR}/library/core
    ${PROJECT_SOURCE_DIR}/library/utility
)


###########
# utility #
###########

set( midge_headers ${midge_headers}
	${util_dir}/initializer.hh
    ${util_dir}/instructable.hh
	${util_dir}/midge_error.hh
	${util_dir}/message_logger.hh
	${util_dir}/shared_cancel.hh
	${util_dir}/signal_slot.hh
    ${util_dir}/signal.hh
	${util_dir}/slot.hh
	${util_dir}/types.hh
	${util_dir}/typenull.hh
	${util_dir}/typelist.hh
	${util_dir}/typelength.hh
	${util_dir}/typeat.hh
	${util_dir}/typechain.hh
	${util_dir}/typerename.hh
	${util_dir}/typereplace.hh
)
set( midge_sources ${midge_sources}
    ${util_dir}/instructable.cc
	${util_dir}/midge_error.cc
	${util_dir}/message_logger.cc
    ${util_dir}/signal.cc
    ${util_dir}/slot.cc
)

########
# core #
########

set( midge_headers ${midge_headers}
	${core_dir}/coremsg.hh
	${core_dir}/stream.hh
	${core_dir}/stream_timer.hh
	${core_dir}/_stream.hh
	${core_dir}/input.hh
	${core_dir}/_input.hh
	${core_dir}/output.hh
	${core_dir}/_output.hh
	${core_dir}/node.hh
	${core_dir}/_buffer.hh
	${core_dir}/_in.hh
	${core_dir}/_out.hh
	${core_dir}/bystander.hh
	${core_dir}/producer.hh
	${core_dir}/consumer.hh
	${core_dir}/transformer.hh
	${core_dir}/diptera.hh
)
set( midge_sources ${midge_sources}
	${core_dir}/coremsg.cc
	${core_dir}/stream.cc
    ${core_dir}/stream_timer.cc
	${core_dir}/input.cc
	${core_dir}/output.cc
	${core_dir}/node.cc
	${core_dir}/bystander.cc
	${core_dir}/producer.cc
	${core_dir}/consumer.cc
	${core_dir}/transformer.cc
	${core_dir}/diptera.cc
)

###########
# library #
###########

pbuilder_library( 
    TARGET Midge 
    SOURCES ${midge_sources}
)

pbuilder_component_install_and_export( 
    COMPONENT Library
    LIBTARGETS Midge
)

pbuilder_install_headers( ${midge_headers} )
