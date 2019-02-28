/*
 * typereplace.hh
 *
 *  Created on: Dec 15, 2018
 *      Author: obla999
 */

#ifndef MIDGE_TYPEREPLACE_HH_
#define MIDGE_TYPEREPLACE_HH_

#include <type_traits>

namespace midge
{

    template< class x_list, class x_find, class x_replace >
    struct type_replace_imp;

    template< template<class...> class x_list, class x_find, class x_replace, class... x_types >
    struct type_replace_imp< x_list<x_types...>, x_find, x_replace >
    {
        using type = x_list<
            typename std::conditional<
                std::is_same< x_find, x_types >::value,
                x_replace,
                x_types
            >::type...
        >;
    };

    template< class x_list, class x_find, class x_replace >
    using type_replace = typename type_replace_imp< x_list, x_find, x_replace>::type;

}

#endif /* MIDGE_TYPEREPLACE_HH_ */
