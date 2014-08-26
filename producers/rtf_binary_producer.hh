#ifndef _midge_rtf_binary_producer_hh_
#define _midge_rtf_binary_producer_hh_

#include "_file_producer.hh"
#include "rtf_data.hh"
#include "binary.hh"

namespace midge
{

    typedef _file_producer< rtf_data, binary > rtf_binary_producer;

}

#endif
