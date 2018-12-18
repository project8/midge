#ifndef _midge_typechain_hh_
#define _midge_typechain_hh_

#include "typeint.hh"
#include "typereplace.hh"

namespace midge
{
    /**
     * The type chain is a chain of inheritance based on the types and positions of those types in a type list.
     *
     * The functionality will be illustrated with an example:
     *
     * You start with a set of types: [int, float, char]
     *
     * You have some prototype class that will be used for the inheritance:
     *   template< class _type, int _index > class my_prototype
     *
     * You will effectively end up with this inheritance chain:
     *
     *   my_prototype< char, 2 >
     *      ^
     *      |
     *   my_prototype< float, 1 >
     *      ^
     *      |
     *   my_prototype< int, 0 >
     *
     * I say "effectively" because the actual chain is somewhat more complicated.  for each level of the chain you have:
     *   1. A type_chain class
     *   2. That type_chain class inherits from my_prototype
     *   3. That type_chain class also inherits from the next level of the chain
     */

    /// The prototype class that gets replaced by a type
    class _type
    {};

    /// The prototype class that gets replaced by a type index (or rather the struct that wraps a type's index)
    class _index
    {};

    /// Template prototype
    template< class x_prototype, int x_index, class... x_types >
    class type_chain;

    // Alias to start a chain
    template< class x_prototype, class... x_types >
    using type_start_chain = type_chain< x_prototype, 0, x_types... >;

    /// Version of type_chain that actually gets used
    template< class x_prototype, int x_index, class x_head, class... x_tail >
    class type_chain< x_prototype, x_index, x_head, x_tail... > :
        public type_replace< type_replace< x_prototype, _type, x_head >, _index, type_int< x_index > >,
        public type_chain< x_prototype, x_index + 1, x_tail... >
    {
        private:
            using this_type = type_replace< type_replace< x_prototype, _type, x_head >, _index, type_int< x_index > >;
            using next_type = type_chain< x_prototype, x_index + 1, x_tail... >;

        public:
            template< class... x_args >
            type_chain( x_args... args ) :
                    this_type( args... ),
                    next_type( args... )
            {
            }

            virtual ~type_chain()
            {
            }
    };

    /// Final level of type_chain, once the set of types has been exhausted
    template< class x_prototype, int x_index >
    class type_chain< x_prototype, x_index >
    {
        public:
            template< class... x_args >
            type_chain( x_args... )
            {
            }

            virtual ~type_chain()
            {
            }
    };

}

#endif
