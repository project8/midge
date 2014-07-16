#include "link.hh"

namespace midge
{

    link::link() :
            f_name( "" )
    {
    }
    link::~link()
    {
    }

    const string& link::get_name() const
    {
        return f_name;
    }
    void link::set_name( const string& p_name )
    {
        f_name = p_name;
        return;
    }

}
