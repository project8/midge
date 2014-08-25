#ifndef _midge_cf_binary_consumer_hh_
#define _midge_cf_binary_consumer_hh_

#include "_file_consumer.hh"
#include "cf_data.hh"
#include "binary.hh"

namespace midge
{

    typedef _file_consumer< cf_data, binary > cf_binary_consumer;

}

#endif
