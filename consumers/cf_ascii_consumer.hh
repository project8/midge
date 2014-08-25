#ifndef _midge_cf_ascii_consumer_hh_
#define _midge_cf_ascii_consumer_hh_

#include "_file_consumer.hh"
#include "cf_data.hh"
#include "ascii.hh"

namespace midge
{

    typedef _file_consumer< cf_data, ascii > cf_ascii_consumer;

}

#endif
