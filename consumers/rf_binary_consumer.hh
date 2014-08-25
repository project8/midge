#ifndef _midge_rf_binary_consumer_hh_
#define _midge_rf_binary_consumer_hh_

#include "_file_consumer.hh"
#include "rf_data.hh"
#include "binary.hh"

namespace midge
{

    typedef _file_consumer< rf_data, binary > rf_binary_consumer;

}

#endif
