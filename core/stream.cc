#include "stream.hh"

namespace midge
{

    const state_t stream::s_none = 0;
    const state_t stream::s_start = 1;
    const state_t stream::s_run = 2;
    const state_t stream::s_stop = 3;
    const state_t stream::s_exit = 4;

    stream::stream()
    {
    }
    stream::~stream()
    {
    }

}
