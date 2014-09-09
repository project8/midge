#include "line.hh"

namespace midge
{

    line::line() :
            f_id( 0 ),
            f_time( 0. ),
            f_frequency( 0. ),
            f_ratio( 0. ),
            f_window( 0. ),
            f_slope( 0. ),
            f_duration( 0. ),
            f_correlation( 0. ),
            f_deviation( 0. ),
            f_local_count( 0 ),
            f_local_sum( 0. ),
            f_local_r_sum( 0. ),
            f_local_rt_sum( 0. ),
            f_local_rf_sum( 0. ),
            f_local_rtt_sum( 0. ),
            f_local_rff_sum( 0. ),
            f_local_rtf_sum( 0. ),
            f_local(),
            f_global_count( 0 ),
            f_global_sum( 0. ),
            f_global_r_sum( 0. ),
            f_global_rt_sum( 0. ),
            f_global_rf_sum( 0. ),
            f_global_rtt_sum( 0. ),
            f_global_rff_sum( 0. ),
            f_global_rtf_sum( 0. ),
            f_global()
    {
    }
    line::~line()
    {
    }

}
