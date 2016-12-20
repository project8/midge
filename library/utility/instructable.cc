/*
 * instructable.cc
 *
 *  Created on: Feb 4, 2016
 *      Author: nsoblath
 */

#include "instructable.hh"

namespace midge
{

    instructable::instructable() :
            f_mutex(),
            f_instruction( instruction::none ),
            f_have_instruction( false )
    {
    }

    instructable::~instructable()
    {
    }

    instruction instructable::use_instruction()
    {
        std::unique_lock< std::mutex > t_lock( f_mutex );
        instruction t_temp = f_instruction;
        f_instruction = instruction::carry_on;
        f_have_instruction = false;
        return t_temp;
    }

} /* namespace midge */
