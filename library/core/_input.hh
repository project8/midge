#ifndef _midge__input_hh_
#define _midge__input_hh_

#include "midge_error.hh"
#include "_stream.hh"
#include "input.hh"

#include "typename.hh"

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
                    throw error() << "input <" << get_name() << "> on node <" << f_node->get_name() << "> cannot cast the provided stream down to the expected specific type\n" <<
                            "Look for a mismatch between the data types in:\n" <<
                            "\tOutput: " << scarab::type( *p_stream ) << '\n' <<
                            "\tInput: " << scarab::type( t_typed_stream );
                }

                t_typed_stream->label() = f_node->get_name() + ":" + get_name();
                (f_node->*f_member)( t_typed_stream );
                return;
            }

        private:
            x_node* f_node;
            void (x_node::*f_member)( _stream< x_type >* );
    };

}

#endif
