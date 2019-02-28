/*
 * typerename.hh
 *
 *  Created on: Dec 15, 2018
 *      Author: N.S. Oblath
 */

#ifndef MIDGE_TYPERENAME_HH_
#define MIDGE_TYPERENAME_HH_

namespace midge
{

    template< class x_a, template<class...> class x_b > struct type_rename_impl;

    template< template<class...> class x_a, class... x_types, template<class...> class x_b >
        struct type_rename_impl< x_a< x_types... >, x_b >
    {
        using type = x_b< x_types... >;
    };

    /// Replaces type A with type B; e.g. type_rename<type_list<int, float>, std::pair> --> std::pair<int, float>
    template< class x_a, template<class...> class x_b >
    using type_rename = typename type_rename_impl< x_a, x_b >::type;

}

#endif /* MIDGE_TYPERENAME_HH_ */
