#ifndef _midge_rtf_ascii_consumer_hh_
#define _midge_rtf_ascii_consumer_hh_

#include "_file_consumer.hh"
#include "rtf_data.hh"
#include "ascii.hh"

namespace midge
{

    typedef _file_consumer< rtf_data, ascii > rtf_ascii_consumer;

}

#endif
