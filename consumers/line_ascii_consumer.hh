#ifndef _midge_line_ascii_consumer_hh_
#define _midge_line_ascii_consumer_hh_

#include "_file_consumer.hh"
#include "line_data.hh"
#include "ascii.hh"

namespace midge
{

    typedef _file_consumer< line_data, ascii > line_ascii_consumer;

}

#endif
