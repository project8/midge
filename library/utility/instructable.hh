/*
 * instructable.hh
 *
 *  Created on: Feb 4, 2016
 *      Author: nsoblath
 */

#ifndef MIDGE_INSTRUCTABLE_HH_
#define MIDGE_INSTRUCTABLE_HH_

#include <atomic>
#include <mutex>

namespace midge
{

    enum class instruction
    {
        carry_on,
        pause,
        resume,
        none
    };

    class instructable
    {
        public:
            instructable();
            virtual ~instructable();

        public:
            void instruct( instruction t_inst );

        protected:
            instruction use_instruction();
            bool have_instruction() const;

        private:
            mutable std::mutex f_mutex;
            instruction f_instruction;
            bool f_have_instruction;

    };

    inline bool instructable::have_instruction() const
    {
        std::unique_lock< std::mutex > t_lock( f_mutex );
        return f_have_instruction;
    }

    inline void instructable::instruct( instruction a_inst )
    {
        std::unique_lock< std::mutex > t_lock( f_mutex );
        f_instruction = a_inst;
        f_have_instruction = true;
        return;
    }



} /* namespace midge */

#endif /* MIDGE_INSTRUCTABLE_HH_ */
