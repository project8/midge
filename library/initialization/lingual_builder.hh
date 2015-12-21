#ifndef _midge_lingual_builder_hh_
#define _midge_lingual_builder_hh_

#include <map>

#include "lingual.hh"
#include "value.hh"
#include "typelist.hh"
using std::map;

namespace midge
{

    typedef typelist_1( string )lingual_types;

    template< class x_type >
    class lingual_builder
    {
        public:
            lingual_builder()
            {
                f_type = new x_type();
            }
            virtual ~lingual_builder()
            {
                delete f_type;
            }

        public:
            void operator()( value* p_value )
            {
                if( p_value->is< ::midge::lingual >() == true )
                {
                    operator()( p_value->as< ::midge::lingual >() );
                }
                else
                {
                    throw error() << "lingual builder for <" << typeid(x_type).name() << "> received unknown value type";
                }
                return;
            }
            void operator()( ::midge::lingual* p_lingual )
            {
                f_type->assign( p_lingual->str() );
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
