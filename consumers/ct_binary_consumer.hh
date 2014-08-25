#ifndef _midge_ct_binary_consumer_hh_
#define _midge_ct_binary_consumer_hh_

#include "_file_consumer.hh"
#include "ct_data.hh"
#include "binary.hh"

namespace midge
{

    typedef _file_consumer< ct_data, binary > ct_binary_consumer;

}

#endif
