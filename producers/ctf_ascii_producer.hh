#ifndef _midge_ctf_ascii_producer_hh_
#define _midge_ctf_ascii_producer_hh_

#include "_file_producer.hh"
#include "ctf_data.hh"
#include "ascii.hh"

namespace midge
{

    typedef _file_producer< ctf_data, ascii > ctf_ascii_producer;

}

#endif
