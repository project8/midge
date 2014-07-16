#ifndef _midge_in_hh_
#define _midge_in_hh_

#include "component.hh"

#include <sstream>
using std::stringstream;

namespace midge
{

    template< class x_type, int x_index >
    class in :
        virtual public component
    {
        public:
            in( node** p_table ) :
                    f_input( NULL ),
                    f_table( p_table )
            {
                stringstream t_name;
                t_name << "set_input_" << x_index;

                input( this, &in< x_type, x_index >::set, t_name.str() );
            }
            virtual ~in()
            {
            }

        public:
            void set( x_type* p_input )
            {
                f_input = p_input;
                f_table[ x_index ] = p_input;
                return;
            }

            x_type* get()
            {
                return f_input;
            }
            const x_type* get() const
            {
                return f_input;
            }

        protected:
            x_type* f_input;
            node** f_table;
    };
}

#endif
