#ifndef _midge_rf_ascii_producer_hh_
#define _midge_rf_ascii_producer_hh_

#include "_file_producer.hh"
#include "rf_data.hh"
#include "ascii.hh"

namespace midge
{

    typedef _file_producer< rf_data, ascii > rf_ascii_producer;

}

#endif
