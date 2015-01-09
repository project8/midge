#ifndef _midge_event_binary_producer_hh_
#define _midge_event_binary_producer_hh_

#include "_file_producer.hh"
#include "event_data.hh"
#include "binary.hh"

namespace midge
{

    typedef _file_producer< event_data, binary > event_binary_producer;

}

#endif
