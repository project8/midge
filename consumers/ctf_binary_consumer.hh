#ifndef _midge_ctf_binary_consumer_hh_
#define _midge_ctf_binary_consumer_hh_

#include "_file_consumer.hh"
#include "ctf_data.hh"
#include "binary.hh"

namespace midge
{

    typedef _file_consumer< ctf_data, binary > ctf_binary_consumer;

}

#endif
