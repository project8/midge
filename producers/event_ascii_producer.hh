#ifndef _midge_event_ascii_producer_hh_
#define _midge_event_ascii_producer_hh_

#include "_file_producer.hh"
#include "event_data.hh"
#include "ascii.hh"

namespace midge
{

    typedef _file_producer< event_data, ascii > event_ascii_producer;

}

#endif
