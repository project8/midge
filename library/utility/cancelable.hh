/*
 * cancelable.hh
 *
 *  Created on: Jan 24, 2016
 *      Author: nsoblath
 *
 *  Base class for thread-safe cancellation
 */

#ifndef MIDGE_CANCELABLE_HH_
#define MIDGE_CANCELABLE_HH_

#include <atomic>

namespace midge
{
    class cancelable
    {
        public:
            cancelable();
            virtual ~cancelable();

            /// asynchronous cancel function
            void cancel();

            /// reset to non-canceled state
            void reset_cancel();

            /// check canceled state
            bool is_canceled() const;

        private:
            virtual void do_cancellation();
            virtual void do_reset_cancellation();

            std::atomic< bool > f_canceled;
    };

    inline void cancelable::cancel()
    {
        f_canceled.store( true );
        this->do_cancellation();
    }

    inline void cancelable::reset_cancel()
    {
        f_canceled.store( false );
        this->do_reset_cancellation();
    }


    inline bool cancelable::is_canceled() const
    {
        return f_canceled.load();
    }

} /* namespace midge */

#endif /* MIDGE_CANCELABLE_HH_ */
