#include "stream.hh"

namespace midge
{

    const command_t stream::s_none = 0;
    const command_t stream::s_start = 1;
    const command_t stream::s_run = 2;
    const command_t stream::s_stop = 3;
    const command_t stream::s_exit = 4;

    stream::stream()
    {
    }
    stream::~stream()
    {
    }

}
