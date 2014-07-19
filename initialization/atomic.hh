#ifndef _midge_atomic_hh_
#define _midge_atomic_hh_

#include "object.hh"
#include "array.hh"
#include "lingual.hh"
#include "numerical.hh"
#include "boolean.hh"
#include "null.hh"

#include "typelist.hh"

#include <map>
using std::map;

namespace midge
{

    //typedef typelist_11( unsigned char, char, unsigned short, short, unsigned int, int, unsigned long, long, float, double, string ) atomic;

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
            void operator()( string& p_string )
            {
                stringstream t_stream;
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
