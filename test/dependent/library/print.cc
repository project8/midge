// print.cc
// Author: N.S. Oblath
// Date: May 4, 2020

#include "print.hh"

#include "logger.hh"

LOGGER( plog, "print" );

namespace pbtest
{
    void print( const scarab::param& a_param )
    {
        LINFO( plog, a_param );
        return;
    }
}
