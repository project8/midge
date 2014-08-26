#ifndef _midge_rtf_ascii_producer_hh_
#define _midge_rtf_ascii_producer_hh_

#include "_file_producer.hh"
#include "rtf_data.hh"
#include "ascii.hh"

namespace midge
{

    typedef _file_producer< rtf_data, ascii > rtf_ascii_producer;

}

#endif
