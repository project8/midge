#ifndef _midge_rt_binary_producer_hh_
#define _midge_rt_binary_producer_hh_

#include "_file_producer.hh"
#include "rt_data.hh"
#include "binary.hh"

namespace midge
{

    typedef _file_producer< rt_data, binary > rt_binary_producer;

}

#endif
