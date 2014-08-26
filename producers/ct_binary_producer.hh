#ifndef _midge_ct_binary_producer_hh_
#define _midge_ct_binary_producer_hh_

#include "_file_producer.hh"
#include "ct_data.hh"
#include "binary.hh"

namespace midge
{

    typedef _file_producer< ct_data, binary > ct_binary_producer;

}

#endif
