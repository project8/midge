#include "null.hh"

namespace midge
{

    null::null()
    {
        set( this );
    }
    null::~null()
    {
    }
    null* null::clone() const
    {
        return (new null());
    }

}
