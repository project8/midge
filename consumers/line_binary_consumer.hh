#ifndef _midge_line_binary_consumer_hh_
#define _midge_line_binary_consumer_hh_

#include "_file_consumer.hh"
#include "line_data.hh"
#include "binary.hh"

namespace midge
{

    typedef _file_consumer< line_data, binary > line_binary_consumer;

}

#endif
