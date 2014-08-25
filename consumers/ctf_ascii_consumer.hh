#ifndef _midge_ctf_ascii_consumer_hh_
#define _midge_ctf_ascii_consumer_hh_

#include "_file_consumer.hh"
#include "ctf_data.hh"
#include "ascii.hh"

namespace midge
{

    typedef _file_consumer< ctf_data, ascii > ctf_ascii_consumer;

}

#endif
