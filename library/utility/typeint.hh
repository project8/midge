#ifndef _midge_typeint_hh_
#define _midge_typeint_hh_

#include <type_traits>

namespace midge
{
    /// Wraps integral constant x_value in a type; value is available as type_int::value
    template< int x_value > using type_int = std::integral_constant< int, x_value >;
}

#endif
