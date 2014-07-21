#ifndef _midge_out_hh_
#define _midge_out_hh_

#include "node.hh"

#include <sstream>
using std::stringstream;

namespace midge
{

    template< class x_type, int x_index >
    class _out :
        virtual public node
    {
        public:
            _out( node** p_table ) :
                    f_out( NULL ),
                    f_table( p_table )
            {
                stringstream t_name;
                t_name << "out_" << x_index;

                output( this, &_out< x_type, x_index >::out, t_name.str() );
            }
            virtual ~_out()
            {
            }

        public:
            void out( x_type* p_output )
            {
                f_out = p_output;
                f_table[ x_index ] = p_output;
                return;
            }

        protected:
            x_type* get()
            {
                return f_out;
            }
            const x_type* get() const
            {
                return f_out;
            }

        private:
            x_type* f_out;
            node** f_table;
    };
}

#endif
