#ifndef _midge_allocator_hh_
#define _midge_allocator_hh_

namespace midge
{

    template< class x_type >
    class allocator
    {
        public:
            typedef x_type value_type;
            typedef x_type& reference;
            typedef x_type* pointer;
            typedef const x_type& const_reference;
            typedef const x_type* const_pointer;

            template< class x_other >
            class rebind
            {
                typedef allocator< x_other > other;
            };

    };

}

#endif
