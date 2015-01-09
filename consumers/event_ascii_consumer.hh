#ifndef _midge_event_ascii_consumer_hh_
#define _midge_event_ascii_consumer_hh_

#include "_file_consumer.hh"
#include "event_data.hh"
#include "ascii.hh"

namespace midge
{

    typedef _file_consumer< event_data, ascii > event_ascii_consumer;

}

#endif
