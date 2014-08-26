#ifndef _midge_cf_binary_producer_hh_
#define _midge_cf_binary_producer_hh_

#include "_file_producer.hh"
#include "cf_data.hh"
#include "binary.hh"

namespace midge
{

    typedef _file_producer< cf_data, binary > cf_binary_producer;

}

#endif
