#ifndef _midge_typeat_hh_
#define _midge_typeat_hh_

#include "typenull.hh"

namespace midge
{
    // The type_at struct give you the type at a particular index within a set of types.
    // e.g. the type at position 1 in [int, short, char] is short.

    template< int x_index, class... x_types >
    struct type_at_impl;

    // recursive case
    template< int x_index, class x_head, class... x_tail >
    struct type_at_impl< x_index, x_head, x_tail... > : type_at_impl< x_index-1, x_tail... >
    { };

    // base case
    template< class x_head, class... x_tail >
    struct type_at_impl< 0, x_head, x_tail... >
    {
        using type = x_head;
    };

    template< int x_index >
    struct type_at_impl< x_index >
    {
        using type = _;
    };

    /// Determines the type at position x_index within parameter pack x_types
    template< int x_index, class... x_types >
    using type_at = typename type_at_impl< x_index, x_types... >::type;


    // This version gives you the type at a position within a type's template parameters
    // e.g. the type at position 1 in type_list<int, short, char> is short
    // This version currently isn't used in midge, but the code has been kept in case it's useful in the future
    /*
    template< int x_index, class x_list >
    struct type_at_impl;

    // recursive case
    template< int x_index, class x_head, class... x_tail >
    struct type_at_impl< x_index, type_list< x_head, x_tail...> > : type_at_impl< x_index-1, type_list<x_tail...> >
    { };

    // base case
    template< class x_head, class... x_tail >
    struct type_at_impl< 0, type_list<x_head, x_tail...> >
    {
        typedef x_head type;
    };

    template< int x_index >
    struct type_at_impl< x_index, type_list<> >
    {
        typedef _ type;
    };

    template< int x_index, class x_list >
    using type_at = typename type_at_impl< x_index, x_list >::type;
    */
}

#endif
