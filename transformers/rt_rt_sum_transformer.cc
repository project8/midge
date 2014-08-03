#include "rt_rt_sum_transformer.hh"

namespace midge
{

    rt_rt_sum_transformer::rt_rt_sum_transformer() :
            f_size( 0 ),
            f_in_0( NULL ),
            f_in_1( NULL ),
            f_out( NULL )
    {
    }
    rt_rt_sum_transformer::~rt_rt_sum_transformer()
    {
    }

    bool rt_rt_sum_transformer::start_transformer()
    {
        if( in< 0 >()->get_size() != in< 1 >()->get_size() )
        {
            throw error() << "rt rt sum transformer inputs must have the same size";
            return false;
        }

        if( in< 0 >()->get_interval() != in< 1 >()->get_interval() )
        {
            throw error() << "rt rt sum transformer inputs must have the same interval";
            return false;
        }

        out< 0 >()->set_size( in< 0 >()->get_size() );
        out< 0 >()->set_interval( in< 0 >()->get_interval() );

        f_size = in< 0 >()->get_size();
        f_in_0 = in< 0 >()->raw();
        f_in_1 = in< 1 >()->raw();
        f_out = out< 0 >()->raw();

        return true;
    }
    bool rt_rt_sum_transformer::execute_transformer()
    {
        for( count_t t_index = 0; t_index < f_size; t_index++ )
        {
            f_out[ t_index ] = f_in_0[ t_index ] + f_in_1[ t_index ];
        }

        out< 0 >()->set_time( in< 0 >()->get_time() );

        return true;
    }
    bool rt_rt_sum_transformer::stop_transformer()
    {
        f_size = 0;
        f_in_0 = NULL;
        f_in_1 = NULL;
        f_out = NULL;

        return true;
    }

}
