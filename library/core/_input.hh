#ifndef _midge__input_hh_
#define _midge__input_hh_

#include "../library/core/_stream.hh"
#include "../library/core/input.hh"
#include "../utility/error.hh"

namespace midge
{

    template< class x_node, class x_type >
    class _input :
        public input
    {
        public:
            _input( x_node* p_node, void (x_node::*p_member)( _stream< x_type >* ) ) :
                f_node( p_node ),
                f_member( p_member )
            {
            }
            virtual ~_input()
            {
            }

        public:
            void set( stream* p_stream )
            {
                stream* t_stream = p_stream;
                _stream< x_type >* t_typed_stream = dynamic_cast< _stream< x_type >* >( t_stream );
                if( t_typed_stream == NULL )
                {
                    throw error() << "input <" << get_name() << "> on node <" << f_node->get_name() << "> cannot cast stream down to specific type";
                }

                (f_node->*f_member)( t_typed_stream );
                return;
            }

        private:
            x_node* f_node;
            void (x_node::*f_member)( _stream< x_type >* );
    };

}

#endif
