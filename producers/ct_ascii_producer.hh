#ifndef _midge_ct_ascii_producer_hh_
#define _midge_ct_ascii_producer_hh_

#include "_file_producer.hh"
#include "ct_data.hh"
#include "ascii.hh"

namespace midge
{

    typedef _file_producer< ct_data, ascii > ct_ascii_producer;

}

#endif
