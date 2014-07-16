#ifndef _midge_out_hh_
#define _midge_out_hh_

#include "component.hh"

#include <sstream>
using std::stringstream;

namespace midge
{

    template< class x_type, int x_index >
    class out :
        virtual public component
    {
        public:
            out( node** p_table ) :
                    f_output( NULL ),
                    f_table( p_table )
            {
                stringstream t_name;
                t_name << "set_output_" << x_index;

                output( this, &out< x_type, x_index >::set, t_name.str() );
            }
            virtual ~out()
            {
            }

        public:
            void set( x_type* p_output )
            {
                f_output = p_output;
                f_table[ x_index ] = p_output;
                return;
            }

            x_type* get()
            {
                return f_output;
            }
            const x_type* get() const
            {
                return f_output;
            }

        protected:
            x_type* f_output;
            node** f_table;
    };
}

#endif
