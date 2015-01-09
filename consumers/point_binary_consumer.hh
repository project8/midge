#ifndef _midge_point_binary_consumer_hh_
#define _midge_point_binary_consumer_hh_

#include "_file_consumer.hh"
#include "point_data.hh"
#include "binary.hh"

namespace midge
{

    typedef _file_consumer< point_data, binary > point_binary_consumer;

}

#endif
