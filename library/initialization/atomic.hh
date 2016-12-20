#ifndef _midge_atomic_hh_
#define _midge_atomic_hh_

#include "typelist.hh"

#include <map>

#include "array.hh"
#include "boolean.hh"
#include "lingual.hh"
#include "null.hh"
#include "numerical.hh"
#include "object.hh"

namespace midge
{

    //typedef typelist_11( unsigned char, char, unsigned short, short, unsigned int, int, unsigned long, long, float, real_t, string ) atomic;

    template< class x_type >
    class atomic
    {
        public:
            atomic()
            {
                f_type = new x_type();
            }
            virtual ~atomic()
            {
                delete f_type;
            }

        public:
            void operator()( std::string& p_string )
            {
                std::stringstream t_stream;
                t_stream << p_string;
                t_stream >> (*f_type);
                return;
            }

            x_type* get()
            {
                return f_type;
            }
            x_type* release()
            {
                x_type* t_type = f_type;
                f_type = NULL;
                return t_type;
            }

        private:
            x_type* f_type;
    };

}

#endif
