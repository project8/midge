#ifndef _midge_typeint_hh_
#define _midge_typeint_hh_

namespace midge
{

    template< int x_result >
    class typeint
    {
        public:
            enum
            {
                result = x_result
            };
    };

}

#endif
