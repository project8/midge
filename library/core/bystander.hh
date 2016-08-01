/*
 * bystander.hh
 *
 *  Created on: Aug 1, 2016
 *      Author: obla999
 */

#ifndef MIDGE_BYSTANDER_HH_
#define MIDGE_BYSTANDER_HH_

#include "instructable.hh"
#include "node.hh"

namespace midge
{

    class bystander :
            virtual public node,
            public instructable
    {
        public:
            bystander();
            virtual ~bystander();
    };

} /* namespace midge */

#endif /* MIDGE_BYSTANDER_HH_ */
