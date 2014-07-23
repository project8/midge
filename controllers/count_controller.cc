#include "count_controller.hh"

namespace midge
{

    count_controller::count_controller() :
            f_count( 0 ),
            f_current( 0 )
    {
    }
    count_controller::~count_controller()
    {
    }

    void count_controller::set_count( const count_t& p_count )
    {
        f_count = p_count;
        return;
    }
    const count_t& count_controller::get_count() const
    {
        return f_count;
    }

    bool count_controller::start_controller_pre()
    {
        f_current = 0;
        return true;
    }
    bool count_controller::execute_controller_pre()
    {
        if( f_current == f_count )
        {
            return false;
        }
        return true;
    }
    bool count_controller::execute_controller_post()
    {
        f_current++;
        return true;
    }
    bool count_controller::stop_controller_post()
    {
        f_current = 0;
        return true;
    }

}
