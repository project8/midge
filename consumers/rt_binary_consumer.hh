#ifndef _midge_rt_binary_consumer_hh_
#define _midge_rt_binary_consumer_hh_

#include "_file_consumer.hh"
#include "rt_data.hh"
#include "binary.hh"

namespace midge
{

    typedef _file_consumer< rt_data, binary > rt_binary_consumer;

}

#endif
