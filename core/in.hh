#ifndef _midge_in_hh_
#define _midge_in_hh_

#include "node.hh"

#include <sstream>
using std::stringstream;

namespace midge
{

    template< class x_type, int x_index >
    class _in :
        virtual public node
    {
        public:
            _in( node** p_table ) :
                    f_in( NULL ),
                    f_table( p_table )
            {
                stringstream t_name;
                t_name << "in_" << x_index;

                node::in( &_in< x_type, x_index >::in, t_name.str() );
            }
            virtual ~_in()
            {
            }

        public:
            void in( x_type* p_in )
            {
                f_in = p_in;
                f_table[ x_index ] = p_in;
                return;
            }

        protected:
            x_type* get()
            {
                return f_in;
            }
            const x_type* get() const
            {
                return f_in;
            }

        private:
            x_type* f_in;
            node** f_table;
    };
}

#endif
