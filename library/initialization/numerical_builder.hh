#ifndef _midge_numerical_builder_hh_
#define _midge_numerical_builder_hh_

#include <map>

#include "numerical.hh"
#include "value.hh"
#include "typelist.hh"

namespace midge
{

    typedef typelist_10( unsigned char, char, unsigned short, short, unsigned int, int, unsigned long, long, float, real_t )numerical_types;

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
                if( p_value->is< ::midge::numerical >() == true )
                {
                    operator()( p_value->as< ::midge::numerical >() );
                }
                else
                {
                    throw error() << "numerical builder for <" << typeid(x_type).name() << "> received unknown value type";
                }
                return;
            }
            void operator()( ::midge::numerical* p_numerical )
            {
                std::stringstream t_stream;
                (void) (t_stream << p_numerical->str());
                (void) (t_stream >> (*f_type));
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
