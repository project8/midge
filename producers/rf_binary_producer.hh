#ifndef _midge_rf_binary_producer_hh_
#define _midge_rf_binary_producer_hh_

#include "_file_producer.hh"
#include "rf_data.hh"
#include "binary.hh"

namespace midge
{

    typedef _file_producer< rf_data, binary > rf_binary_producer;

}

#endif
