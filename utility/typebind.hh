#ifndef _midge_typebind_hh_
#define _midge_typebind_hh_

namespace midge
{

    template< class x_operand, class x_find, class x_replace >
    class typebind
    {
        public:
            typedef x_operand result;
    };

    template< class x_find, class x_replace >
    class typebind< x_find, x_find, x_replace >
    {
        public:
            typedef x_replace result;
    };

    template< template< class > class x_outer, class x_a1, class x_find, class x_replace >
    class typebind< x_outer< x_a1 >, x_find, x_replace >
    {
        public:
            typedef x_outer< typename typebind< x_a1, x_find, x_replace >::result > result;
    };

    template< template< class, class > class x_outer, class x_a1, class x_a2, class x_find, class x_replace >
    class typebind< x_outer< x_a1, x_a2 >, x_find, x_replace >
    {
        public:
            typedef x_outer< typename typebind< x_a1, x_find, x_replace >::result, typename typebind< x_a2, x_find, x_replace >::result > result;
    };

    template< template< class, class, class > class x_outer, class x_a1, class x_a2, class x_a3, class x_find, class x_replace >
    class typebind< x_outer< x_a1, x_a2, x_a3 >, x_find, x_replace >
    {
        public:
            typedef x_outer< typename typebind< x_a1, x_find, x_replace >::result, typename typebind< x_a2, x_find, x_replace >::result, typename typebind< x_a3, x_find, x_replace >::result > result;
    };

    template< template< class, class, class, class > class x_outer, class x_a1, class x_a2, class x_a3, class x_a4, class x_find, class x_replace >
    class typebind< x_outer< x_a1, x_a2, x_a3, x_a4 >, x_find, x_replace >
    {
        public:
            typedef x_outer< typename typebind< x_a1, x_find, x_replace >::result, typename typebind< x_a2, x_find, x_replace >::result, typename typebind< x_a3, x_find, x_replace >::result, typename typebind< x_a4, x_find, x_replace >::result > result;
    };

    template< template< class, class, class, class, class > class x_outer, class x_a1, class x_a2, class x_a3, class x_a4, class x_a5, class x_find, class x_replace >
    class typebind< x_outer< x_a1, x_a2, x_a3, x_a4, x_a5 >, x_find, x_replace >
    {
        public:
            typedef x_outer< typename typebind< x_a1, x_find, x_replace >::result, typename typebind< x_a2, x_find, x_replace >::result, typename typebind< x_a3, x_find, x_replace >::result, typename typebind< x_a4, x_find, x_replace >::result, typename typebind< x_a5, x_find, x_replace >::result > result;
    };

}



#endif
