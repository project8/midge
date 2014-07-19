#ifndef _midge_numerical_builder_hh_
#define _midge_numerical_builder_hh_

#include "value.hh"
#include "numerical.hh"
#include "typelist.hh"

#include <map>
using std::map;

namespace midge
{

    typedef typelist_10( unsigned char, char, unsigned short, short, unsigned int, int, unsigned long, long, float, double ) numerical_types;

    template< class x_type >
    class numerical_builder
    {
        public:
            numerical_builder()
            {
                f_type = new x_type();
            }
            virtual ~numerical_builder()
            {
                delete f_type;
            }

        public:
            void operator()( value* p_value )
            {
                if( p_value->is< numerical >() == true )
                {
                    operator()( p_value->as< numerical >() );
                }
                else
                {
                    throw error() << "numerical builder for <" << typeid(x_type).name() << "> received unknown value type";
                }
                return;
            }
            void operator()( numerical* p_numerical )
            {
                stringstream t_stream;
                t_stream << p_numerical->str();
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
