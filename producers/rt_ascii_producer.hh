#ifndef _midge_rt_ascii_producer_hh_
#define _midge_rt_ascii_producer_hh_

#include "_file_producer.hh"
#include "rt_data.hh"
#include "ascii.hh"

namespace midge
{

    typedef _file_producer< rt_data, ascii > rt_ascii_producer;

}

#endif
