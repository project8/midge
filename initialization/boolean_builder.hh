#ifndef _midge_boolean_builder_hh_
#define _midge_boolean_builder_hh_

#include "value.hh"
#include "boolean.hh"
#include "typelist.hh"

#include <map>
using std::map;

namespace midge
{

    typedef typelist_1( bool ) boolean_types;

    template< class x_type >
    class boolean_builder
    {
        public:
            boolean_builder()
            {
                f_type = new x_type();
            }
            virtual ~boolean_builder()
            {
                delete f_type;
            }

        public:
            void operator()( value* p_value )
            {
                if( p_value->is< boolean >() == true )
                {
                    operator()( p_value->as< boolean >() );
                }
                else
                {
                    throw error() << "boolean builder for <" << typeid(x_type).name() << "> received unknown value type";
                }
                return;
            }
            void operator()( boolean* p_boolean )
            {
                stringstream t_stream;
                t_stream << p_boolean->str();
                t_stream >> (*f_type);
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
