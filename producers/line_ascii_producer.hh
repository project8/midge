#ifndef _midge_line_ascii_producer_hh_
#define _midge_line_ascii_producer_hh_

#include "_file_producer.hh"
#include "line_data.hh"
#include "ascii.hh"

namespace midge
{

    typedef _file_producer< line_data, ascii > line_ascii_producer;

}

#endif
