#ifndef _midge_rt_ascii_consumer_hh_
#define _midge_rt_ascii_consumer_hh_

#include "_file_consumer.hh"
#include "rt_data.hh"
#include "ascii.hh"

namespace midge
{

    typedef _file_consumer< rt_data, ascii > rt_ascii_consumer;

}

#endif
