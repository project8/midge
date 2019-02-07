/*
 * slot.hh
 *
 *  Created on: Feb 6, 2019
 *      Author: N.S. Oblath
 */

#ifndef MIDGE_UTILITY_SLOT_HH_
#define MIDGE_UTILITY_SLOT_HH_

#include <string>

namespace midge
{
    class node;

    class slot
    {
        public:
            slot( const std::string& name, node* owner = nullptr );
            virtual ~slot();
    };

} /* namespace midge */

#endif /* MIDGE_UTILITY_SLOT_HH_ */
