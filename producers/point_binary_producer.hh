#ifndef _midge_point_binary_producer_hh_
#define _midge_point_binary_producer_hh_

#include "_file_producer.hh"
#include "point_data.hh"
#include "binary.hh"

namespace midge
{

    typedef _file_producer< point_data, binary > point_binary_producer;

}

#endif
