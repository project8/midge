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

    template< class A, template<class...> class B > struct type_rename_impl;

    template< template<class...> class A, class... T, template<class...> class B >
        struct type_rename_impl< A< T... >, B >
    {
        using type = B< T... >;
    };

    /// Replaces type A with type B; e.g. type_rename<type_list<int, float>, std::pair> --> std::pair<int, float>
    template< class A, template<class...> class B >
    using type_rename = typename type_rename_impl< A, B >::type;

}

#endif /* MIDGE_TYPERENAME_HH_ */
