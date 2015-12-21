#ifndef _midge_in_hh_
#define _midge_in_hh_

#include <sstream>

#include "../library/core/_input.hh"
#include "../library/core/_stream.hh"
#include "../library/core/node.hh"
#include "../utility/typenull.hh"
using std::stringstream;

#include <vector>
using std::vector;

namespace midge
{

    template< class x_type, class x_index >
    class _in :
        virtual public node
    {
        public:
            _in() :
                    f_in( NULL )
            {
                stringstream t_name;
                t_name << "in_" << x_index::result;
                node::in( new _input< _in< x_type, x_index >, x_type >( this, &_in< x_type, x_index >::in ), t_name.str() );
            }
            virtual ~_in()
            {
            }

        public:
            void in( _stream< x_type >* p_in )
            {
                f_in = p_in;
                return;
            }

        protected:
            _stream< x_type >& get_stream()
            {
                return *f_in;
            }

        private:
            _stream< x_type >* f_in;
    };
}

#endif
