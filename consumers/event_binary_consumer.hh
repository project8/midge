#ifndef _midge_event_binary_consumer_hh_
#define _midge_event_binary_consumer_hh_

#include "_file_consumer.hh"
#include "event_data.hh"
#include "binary.hh"

namespace midge
{

    typedef _file_consumer< event_data, binary > event_binary_consumer;

}

#endif
