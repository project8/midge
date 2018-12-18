#ifndef _midge_typelength_hh_
#define _midge_typelength_hh_

#include "typerename.hh"

#include <type_traits>

namespace midge
{
    /// Calculates the number of types in template pack x_types; returns it as type_length::value
    template< class... x_types > using type_length = std::integral_constant< std::size_t, sizeof...(x_types) >;

    /// Calculates the number of types in type list x_list; returns it as type_size::value
    template< class x_list > using type_size = type_rename< x_list, type_length >;

}

#endif
