#ifndef _midge_null_builder_hh_
#define _midge_null_builder_hh_

#include "value.hh"
#include "null.hh"
#include "typelist.hh"

#include <map>
using std::map;

namespace midge
{

    typedef typelist_0() null_types;

    template< class x_type >
    class null_builder
    {
        public:
            null_builder()
            {
                f_type = new x_type();
            }
            virtual ~null_builder()
            {
                delete f_type;
            }

        public:
            void operator()( value* p_value )
            {
                if( p_value->is< null >() == true )
                {
                    operator()( p_value->as< null >() );
                }
                else
                {
                    throw error() << "null builder for <" << typeid(x_type).name() << "> received unknown value type";
                }
                return;
            }
            void operator()( null* p_null )
            {
                return;
            }

            x_type* operator()()
            {
                return f_type;
            }

        private:
            x_type* f_type;
    };

}

#endif
