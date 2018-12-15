# CMakeLists.txt for midge/library

set( core_dir core )
set( bind_dir bindings )
set( init_dir initialization )
set( util_dir utility )

###########
# utility #
###########

set( midge_headers ${midge_headers}
	${util_dir}/initializer.hh
	${util_dir}/midge_error.hh
	${util_dir}/message_logger.hh
	${util_dir}/instructable.hh
	${util_dir}/shared_cancel.hh
	${util_dir}/types.hh
	${util_dir}/typenull.hh
	${util_dir}/typenode.hh
	${util_dir}/typelist.hh
	${util_dir}/typelength.hh
	${util_dir}/typeat.hh
	${util_dir}/typein.hh
	${util_dir}/typeif.hh
	${util_dir}/typechain.hh
)
set( midge_sources ${midge_sources}
	${util_dir}/midge_error.cc
	${util_dir}/message_logger.cc
	${util_dir}/instructable.cc
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

pbuilder_library( Midge midge_sources "" )
pbuilder_install_headers( ${midge_headers} )
