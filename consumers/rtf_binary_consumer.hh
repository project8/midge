#ifndef _midge_rtf_binary_consumer_hh_
#define _midge_rtf_binary_consumer_hh_

#include "_file_consumer.hh"
#include "rtf_data.hh"
#include "binary.hh"

namespace midge
{

    typedef _file_consumer< rtf_data, binary > rtf_binary_consumer;

}

#endif
