#ifndef _midge_rf_ascii_consumer_hh_
#define _midge_rf_ascii_consumer_hh_

#include "_file_consumer.hh"
#include "rf_data.hh"
#include "ascii.hh"

namespace midge
{

    typedef _file_consumer< rf_data, ascii > rf_ascii_consumer;

}

#endif
