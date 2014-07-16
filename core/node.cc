#include "node.hh"

namespace midge
{

    node::node() :
            f_name( "" )
    {
    }
    node::~node()
    {
    }

    const string& node::get_name() const
    {
        return f_name;
    }
    void node::set_name( const string& p_name )
    {
        f_name = p_name;
        return;
    }

}
