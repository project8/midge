#ifndef _midge_in_hh_
#define _midge_in_hh_

#include "_input.hh"
#include "_stream.hh"
#include "node.hh"

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
                node::in( new _input< _in< x_type, x_index >, x_type >( this, &_in< x_type, x_index >::in ), "in_"+std::to_string(x_index::value) );
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
