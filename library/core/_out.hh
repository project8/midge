#ifndef _midge_out_hh_
#define _midge_out_hh_

#include <sstream>

#include "_buffer.hh"
#include "_output.hh"
#include "_stream.hh"
#include "node.hh"
#include "typenull.hh"

#include <vector>

namespace midge
{

    template< class x_type, class x_index >
    class _out :
        virtual public node
    {
        public:
            _out() :
                    f_buffer( new _buffer< x_type >( this ) )
            {
                std::stringstream t_name_str;
                t_name_str << "out_" << x_index::result;
                std::string t_name = t_name_str.str();
                node::out( new _output< _out< x_type, x_index >, x_type >( this, &_out< x_type, x_index >::out ), t_name );
                f_buffer->set_write_stream_name( t_name );
            }
            virtual ~_out()
            {
                delete f_buffer;
            }

        public:
            _stream< x_type >* out()
            {
                return f_buffer->read();
            }

        protected:
            _buffer< x_type >& get_buffer()
            {
                return *(f_buffer);
            }
            _stream< x_type >& get_stream()
            {
                return *(f_buffer->write());
            }

        private:
            _buffer< x_type >* f_buffer;
    };
}

#endif
