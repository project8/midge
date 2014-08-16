#ifndef _midge__output_hh_
#define _midge__output_hh_

#include "output.hh"
#include "error.hh"
#include "_stream.hh"

namespace midge
{

    template< class x_node, class x_type >
    class _output :
        public output
    {
        public:
            _output( x_node* p_node, const _stream< x_type >* (x_node::*p_member)() ) :
                f_node( p_node ),
                f_member( p_member )
            {
            }
            virtual ~_output()
            {
            }

        public:
            const stream* get()
            {
                const _stream< x_type >* t_typed_stream = (f_node->*f_member)();
                const stream* t_stream = dynamic_cast< const stream* >( t_typed_stream );
                if( t_stream == NULL )
                {
                    throw error() << "output <" << get_name() << "> on node <" << f_node->get_name() << "> cannot cast stream up from specific type";
                }

                return t_stream;
            }

        private:
            x_node* f_node;
            const _stream< x_type >* (x_node::*f_member)();
    };

}

#endif
