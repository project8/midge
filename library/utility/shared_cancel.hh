/*
 * shared_cancel.hh
 *
 *  Created on: Jan 29, 2016
 *      Author: nsoblath
 */

#ifndef MIDGE_SHARED_CANCEL_HH_
#define MIDGE_SHARED_CANCEL_HH_

#include <atomic>
#include <memory>

namespace midge
{
    typedef std::shared_ptr< std::atomic< bool > > shared_cancel_t;
}

#endif /* MIDGE_SHARED_CANCEL_HH_ */
