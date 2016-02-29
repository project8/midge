#ifndef _midge__output_hh_
#define _midge__output_hh_

#include "midge_error.hh"
#include "_stream.hh"
#include "output.hh"

namespace midge
{

    template< class x_node, class x_type >
    class _output :
        public output
    {
        public:
            _output( x_node* p_node, _stream< x_type >* (x_node::*p_member)() ) :
                f_node( p_node ),
                f_member( p_member )
            {
            }
            virtual ~_output()
            {
            }

        public:
            stream* get()
            {
                _stream< x_type >* t_typed_stream = (f_node->*f_member)();
                stream* t_stream = dynamic_cast< stream* >( t_typed_stream );
                if( t_stream == NULL )
                {
                    throw error() << "output <" << get_name() << "> on node <" << f_node->get_name() << "> cannot cast stream up from specific type";
                }
                t_typed_stream->label() = f_node->get_name() + ":" + get_name() + "(out)";

                return t_stream;
            }

        private:
            x_node* f_node;
            _stream< x_type >* (x_node::*f_member)();
    };

}

#endif
