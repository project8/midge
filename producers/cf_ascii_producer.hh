#ifndef _midge_cf_ascii_producer_hh_
#define _midge_cf_ascii_producer_hh_

#include "_file_producer.hh"
#include "cf_data.hh"
#include "ascii.hh"

namespace midge
{

    typedef _file_producer< cf_data, ascii > cf_ascii_producer;

}

#endif
