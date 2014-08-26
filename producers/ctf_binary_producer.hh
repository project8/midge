#ifndef _midge_ctf_binary_producer_hh_
#define _midge_ctf_binary_producer_hh_

#include "_file_producer.hh"
#include "ctf_data.hh"
#include "binary.hh"

namespace midge
{

    typedef _file_producer< ctf_data, binary > ctf_binary_producer;

}

#endif
