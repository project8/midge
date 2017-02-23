#include "stream.hh"

namespace midge
{

    const enum_t stream::s_none = 0;
    const enum_t stream::s_start = 1;
    const enum_t stream::s_run = 2;
    const enum_t stream::s_stop = 3;
    const enum_t stream::s_exit = 4;
    const enum_t stream::s_error = 100;

    stream::stream()
    {
    }
    stream::~stream()
    {
    }

}
