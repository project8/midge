#ifndef _midge_point_ascii_producer_hh_
#define _midge_point_ascii_producer_hh_

#include "_file_producer.hh"
#include "point_data.hh"
#include "ascii.hh"

namespace midge
{

    typedef _file_producer< point_data, ascii > point_ascii_producer;

}

#endif
