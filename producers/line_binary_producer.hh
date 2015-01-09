#ifndef _midge_line_binary_producer_hh_
#define _midge_line_binary_producer_hh_

#include "_file_producer.hh"
#include "line_data.hh"
#include "binary.hh"

namespace midge
{

    typedef _file_producer< line_data, binary > line_binary_producer;

}

#endif
