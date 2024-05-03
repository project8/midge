#ifndef _midge_error_hh_
#define _midge_error_hh_

#include "base_exception.hh"

namespace midge
{

    class error : public scarab::typed_exception< error >
    {
        public:
            error();
            error( const error& p_copy );
            error& operator=( const error& p_copy );
            virtual ~error() noexcept;
    };

    // midge should exit and not be restarted
    class node_fatal_error : public  scarab::typed_exception< error >
    {
        public:
            node_fatal_error();
            node_fatal_error( const node_fatal_error& p_copy );
            node_fatal_error& operator=( const node_fatal_error& p_copy );
            virtual ~node_fatal_error() noexcept;
    };

    // midge should exit but could be restarted
    class node_nonfatal_error : public scarab::typed_exception< error >
    {
        public:
            node_nonfatal_error();
            node_nonfatal_error( const node_nonfatal_error& p_copy );
            node_nonfatal_error& operator=( const node_nonfatal_error& p_copy );
            virtual ~node_nonfatal_error() noexcept;
    };

}

#endif
