/*
 * cancelable.cc
 *
 *  Created on: Jan 24, 2016
 *      Author: nsoblath
 */

#include "cancelable.hh"

namespace midge
{

    cancelable::cancelable() :
            f_canceled( false )
    {
    }

    cancelable::~cancelable()
    {
    }

    void cancelable::do_cancellation()
    {
        return;
    }

    void cancelable::do_reset_cancellation()
    {
        return;
    }

} /* namespace midge */
