#ifndef _midge_point_ascii_consumer_hh_
#define _midge_point_ascii_consumer_hh_

#include "_file_consumer.hh"
#include "point_data.hh"
#include "ascii.hh"

namespace midge
{

    typedef _file_consumer< point_data, ascii > point_ascii_consumer;

}

#endif
