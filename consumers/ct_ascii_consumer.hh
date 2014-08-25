#ifndef _midge_ct_ascii_consumer_hh_
#define _midge_ct_ascii_consumer_hh_

#include "_file_consumer.hh"
#include "ct_data.hh"
#include "ascii.hh"

namespace midge
{

    typedef _file_consumer< ct_data, ascii > ct_ascii_consumer;

}

#endif
