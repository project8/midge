#include "line.hh"
#include "coremsg.hh"

#include <cmath>

namespace midge
{

    real_t line::s_window = 0.;
    real_t line::s_width = 0.;
    count_t line::s_count = 0;

    point_data* line::s_data = NULL;

    line::line() :
            f_id( 0 ),
            f_count( 0 ),
            f_score( 0. ),
            f_quality( 0. ),
            f_time( 0. ),
            f_frequency( 0. ),
            f_slope( 0. ),
            f_duration( 0. ),
            f_correlation( 0. ),
            f_deviation( 0. ),
            f_points(),
            f_low(),
            f_low_count( 0 ),
            f_high(),
            f_high_count( 0 ),
            f_line(),
            f_line_count( 0 ),
            f_r_sum( 0. ),
            f_rt_sum( 0. ),
            f_rf_sum( 0. ),
            f_rtt_sum( 0. ),
            f_rff_sum( 0. ),
            f_rtf_sum( 0. )
    {
    }
    line::line( const line& a_copy ) :
            f_id( a_copy.f_id ),
            f_count( a_copy.f_count ),
            f_score( a_copy.f_score ),
            f_quality( a_copy.f_quality ),
            f_time( a_copy.f_time ),
            f_frequency( a_copy.f_frequency ),
            f_slope( a_copy.f_slope ),
            f_duration( a_copy.f_duration ),
            f_correlation( a_copy.f_correlation ),
            f_deviation( a_copy.f_deviation ),
            f_points( a_copy.f_points ),
            f_low( a_copy.f_low ),
            f_low_count( a_copy.f_low_count ),
            f_high( a_copy.f_high ),
            f_high_count( a_copy.f_high_count ),
            f_line( a_copy.f_line ),
            f_line_count( a_copy.f_line_count ),
            f_r_sum( a_copy.f_r_sum ),
            f_rt_sum( a_copy.f_rt_sum ),
            f_rf_sum( a_copy.f_rf_sum ),
            f_rtt_sum( a_copy.f_rtt_sum ),
            f_rff_sum( a_copy.f_rff_sum ),
            f_rtf_sum( a_copy.f_rtf_sum )
    {
    }
    line::~line()
    {
    }

    void line::initialize( const count_t& p_index )
    {
        //msg_warning( coremsg, "line <" << f_id << "> initializing:" << eom );

        f_score = 0.;
        f_quality = 0.;
        f_time = s_data->at( p_index ).time();
        f_frequency = s_data->at( p_index ).frequency();
        f_slope = 0.;
        f_duration = 0.;
        f_correlation = 0.;
        f_deviation = 0.;
        f_points.clear();
        f_low.clear();
        f_low_count = 0;
        f_high.clear();
        f_high_count = 0;
        f_line.clear();
        f_line_count = 0;
        f_r_sum = 0.;
        f_rt_sum = 0.;
        f_rf_sum = 0.;
        f_rtt_sum = 0.;
        f_rff_sum = 0.;
        f_rtf_sum = 0.;

        return;
    }

    void line::update()
    {
        //msg_warning( coremsg, "line <" << f_id << "> updating:" << eom );

        point_it t_point_it;
        group_it t_group_it;

        count_t t_point_id;
        real_t t_point_time = s_data->get_time_interval() * s_data->get_time_index();
        real_t t_point_duration = t_point_time - f_time;
        real_t t_point_frequency;
        real_t t_point_ratio;
        real_t t_point_score;

        real_t t_update_frequency = f_frequency;
        real_t t_update_slope = f_slope;
        real_t t_update_deviation = f_deviation;
        real_t t_update_correlation = f_correlation;
        real_t t_update_center = t_update_frequency + t_update_slope * t_point_duration;
        real_t t_update_min = t_update_center - s_window;
        real_t t_update_max = t_update_center + s_window;
        real_t t_update_low_score = 0.;
        real_t t_update_high_score = 0.;
        real_t t_update_global_score = 0;

        real_t t_update_tt_stat;
        real_t t_update_ff_stat;
        real_t t_update_tf_stat;
        real_t t_update_ttf_stat;
        real_t t_update_dev_stat;

        count_t t_index;
        pair< count_t, count_t > t_current_indices;
        pair< count_t, count_t > t_next_indices;

        t_current_indices.first = (count_t) (floor( t_update_min / s_data->get_frequency_interval() ));
        if( t_current_indices.first < s_data->get_frequency_index() )
        {
            t_current_indices.first = s_data->get_frequency_index();
        }

        t_current_indices.second = (index_t) (ceil( t_update_max / s_data->get_frequency_interval() ));
        if( t_current_indices.second > s_data->get_frequency_index() + s_data->get_size() - 1 )
        {
            t_current_indices.second = s_data->get_frequency_index() + s_data->get_size() - 1;
        }

        point_list t_loop_low;
        count_t t_loop_low_count;

        point_list t_loop_high;
        count_t t_loop_high_count;

        real_t t_loop_r_sum;
        real_t t_loop_rt_sum;
        real_t t_loop_rf_sum;
        real_t t_loop_rtt_sum;
        real_t t_loop_rff_sum;
        real_t t_loop_rtf_sum;

        while( true )
        {
            //msg_warning( coremsg, "  accumulating:" << eom );
            //msg_warning( coremsg, "    center is <" << t_update_center << ">" << eom );
            //msg_warning( coremsg, "    min is <" << t_update_min << ">" << eom );
            //msg_warning( coremsg, "    max is <" << t_update_max << ">" << eom );
            //msg_warning( coremsg, "    first is <" << t_current_indices.first << ">" << eom );
            //msg_warning( coremsg, "    last is <" << t_current_indices.second << ">" << eom );

            if( t_current_indices.first > t_current_indices.second )
            {
                f_score = -1000.;
                return;
            }

            t_loop_low.clear();
            t_loop_low_count = f_low_count;

            t_loop_high.clear();
            t_loop_high_count = f_high_count;

            t_loop_r_sum = f_r_sum;
            t_loop_rt_sum = f_rt_sum;
            t_loop_rf_sum = f_rf_sum;
            t_loop_rtt_sum = f_rtt_sum;
            t_loop_rff_sum = f_rff_sum;
            t_loop_rtf_sum = f_rtf_sum;

            for( t_index = t_current_indices.first; t_index <= t_current_indices.second; t_index++ )
            {
                point& t_loop_point = s_data->at( t_index - s_data->get_frequency_index() );
                t_point_id = t_loop_point.id();
                t_point_frequency = t_loop_point.frequency();
                t_point_ratio = t_loop_point.ratio();
                t_point_score = t_loop_point.score();

                //msg_warning( coremsg, "    point:" << eom );
                //msg_warning( coremsg, "      time is <" << t_point_time << ">" << eom );
                //msg_warning( coremsg, "      frequency is <" << t_point_frequency << ">" << eom );
                //msg_warning( coremsg, "      ratio is <" << t_point_ratio << ">" << eom );

                if( t_point_frequency < t_update_min )
                {
                    continue;
                }
                if( t_point_frequency > t_update_max )
                {
                    continue;
                }

                if( t_point_score < 0. )
                {
                    //msg_warning( coremsg, "      point low" << eom );
                    t_loop_low_count += 1;
                    t_loop_low.push_back( t_loop_point );
                }
                else
                {
                    //msg_warning( coremsg, "      point high" << eom );
                    t_loop_high_count += 1;
                    t_loop_r_sum += t_point_ratio;
                    t_loop_rt_sum += t_point_ratio * t_point_duration;
                    t_loop_rf_sum += t_point_ratio * t_point_frequency;
                    t_loop_rtt_sum += t_point_ratio * t_point_duration * t_point_duration;
                    t_loop_rff_sum += t_point_ratio * t_point_frequency * t_point_frequency;
                    t_loop_rtf_sum += t_point_ratio * t_point_duration * t_point_frequency;
                    t_loop_high.push_back( t_loop_point );
                }
            }

            if( t_loop_high.size() == 0 )
            {
                break;
            }

            //msg_warning( coremsg, "    total count is <" << t_loop_total_count << ">" << eom );
            //msg_warning( coremsg, "    total r sum is <" << t_loop_total_r_sum << ">" << eom );
            //msg_warning( coremsg, "    total rt sum is <" << t_loop_total_rt_sum << ">" << eom );
            //msg_warning( coremsg, "    total rf sum is <" << t_loop_total_rf_sum << ">" << eom );
            //msg_warning( coremsg, "    total rtt sum is <" << t_loop_total_rtt_sum << ">" << eom );
            //msg_warning( coremsg, "    total rff sum is <" << t_loop_total_rff_sum << ">" << eom );
            //msg_warning( coremsg, "    total rtf sum is <" << t_loop_total_rtf_sum << ">" << eom );

            t_update_tt_stat = t_loop_r_sum * t_loop_rtt_sum - t_loop_rt_sum * t_loop_rt_sum;
            t_update_ff_stat = t_loop_r_sum * t_loop_rff_sum - t_loop_rf_sum * t_loop_rf_sum;
            t_update_tf_stat = t_loop_r_sum * t_loop_rtf_sum - t_loop_rt_sum * t_loop_rf_sum;
            t_update_ttf_stat = t_loop_rtt_sum * t_loop_rf_sum - t_loop_rt_sum * t_loop_rtf_sum;

            //msg_warning( coremsg, "    update tt stat is <" << t_update_tt_stat << ">" << eom );
            //msg_warning( coremsg, "    update ff stat is <" << t_update_ff_stat << ">" << eom );
            //msg_warning( coremsg, "    update tf stat is <" << t_update_tf_stat << ">" << eom );
            //msg_warning( coremsg, "    update ttf stat is <" << t_update_ttf_stat << ">" << eom );

            t_update_frequency = t_update_ttf_stat / t_update_tt_stat;
            if( t_update_frequency != t_update_frequency )
            {
                t_update_frequency = t_loop_rf_sum / t_loop_r_sum;
            }
            //msg_warning( coremsg, "    frequency is <" << t_update_frequency << ">" << eom );

            t_update_slope = t_update_tf_stat / t_update_tt_stat;
            if( t_update_slope != t_update_slope )
            {
                t_update_slope = 0.;
            }
            //msg_warning( coremsg, "    slope is <" << t_update_slope << ">" << eom );

            t_update_dev_stat = t_loop_rff_sum;
            t_update_dev_stat += t_update_slope * t_update_slope * t_loop_rtt_sum;
            t_update_dev_stat += t_update_frequency * t_update_frequency * t_loop_r_sum;
            t_update_dev_stat -= 2. * t_update_slope * t_loop_rtf_sum;
            t_update_dev_stat -= 2. * t_update_frequency * t_loop_rf_sum;
            t_update_dev_stat += 2. * t_update_slope * t_update_frequency * t_loop_rt_sum;

            t_update_correlation = sqrt( (t_update_tf_stat * t_update_tf_stat) / (t_update_tt_stat * t_update_ff_stat) );
            if( t_update_correlation != t_update_correlation )
            {
                t_update_correlation = 0.;
            }
            else if( t_update_correlation > 1. )
            {
                t_update_correlation = 0.;
            }
            //msg_warning( coremsg, "    correlation is <" << t_update_correlation << ">" << eom );

            t_update_deviation = sqrt( t_update_dev_stat / t_loop_r_sum );
            if( t_update_deviation != t_update_deviation )
            {
                t_update_deviation = 0.;
            }
            //msg_warning( coremsg, "    deviation is <" << t_update_deviation << ">" << eom );

            t_update_center = t_update_frequency + t_update_slope * t_point_duration;
            t_update_min = t_update_center - s_window;
            t_update_max = t_update_center + s_window;

            t_next_indices.first = (count_t) (floor( t_update_min / s_data->get_frequency_interval() ));
            if( t_next_indices.first < s_data->get_frequency_index() )
            {
                t_next_indices.first = s_data->get_frequency_index();
            }

            t_next_indices.second = (index_t) (ceil( t_update_max / s_data->get_frequency_interval() ));
            if( t_next_indices.second > s_data->get_frequency_index() + s_data->get_size() - 1 )
            {
                t_next_indices.second = s_data->get_frequency_index() + s_data->get_size() - 1;
            }

            if( t_current_indices == t_next_indices )
            {
                break;
            }
            else
            {
                t_current_indices = t_next_indices;
            }
        }

        for( t_index = t_current_indices.first; t_index <= t_current_indices.second; t_index++ )
        {
            s_data->at( t_index - s_data->get_frequency_index() ).id() = f_id;
            s_data->at( t_index - s_data->get_frequency_index() ).ratio() = 0.;
            s_data->at( t_index - s_data->get_frequency_index() ).update();
        }

        f_frequency = t_update_frequency;
        f_slope = t_update_slope;
        f_duration = t_point_duration;
        f_correlation = t_update_correlation;
        f_deviation = t_update_deviation;

        f_low.push_back( t_loop_low );
        f_low_count = t_loop_low_count;
        f_high.push_back( t_loop_high );
        f_high_count = t_loop_high_count;

        f_r_sum = t_loop_r_sum;
        f_rt_sum = t_loop_rt_sum;
        f_rf_sum = t_loop_rf_sum;
        f_rtt_sum = t_loop_rtt_sum;
        f_rff_sum = t_loop_rff_sum;
        f_rtt_sum = t_loop_rtt_sum;
        f_rtf_sum = t_loop_rtf_sum;

        //msg_warning( coremsg, "  trimming:" << eom );
        if( (f_low_count + f_high_count - f_low.front().size() - f_high.front().size()) >= s_count )
        {
            f_low_count -= f_low.front().size();
            while( f_low.front().empty() == false )
            {
                f_low.front().pop_front();
            }
            f_low.pop_front();

            f_high_count -= f_high.front().size();
            while( f_high.front().empty() == false )
            {
                f_points.push_back( f_high.front().front() );
                f_high.front().pop_front();
            }
            f_high.pop_front();
        }
        f_count = f_points.size();

        //msg_warning( coremsg, "  updating:" << eom );
        for( t_group_it = f_low.begin(); t_group_it != f_low.end(); t_group_it++ )
        {
            for( t_point_it = t_group_it->begin(); t_point_it != t_group_it->end(); t_point_it++ )
            {
                t_point_time = (*t_point_it).time();
                t_point_frequency = (*t_point_it).frequency();
                t_point_ratio = (*t_point_it).ratio();
                t_point_score = (*t_point_it).score();
                t_update_low_score += t_point_score;
                //msg_warning( coremsg, "    low:" << eom );
                //msg_warning( coremsg, "      time is <" << t_point_time << ">" << eom );
                //msg_warning( coremsg, "      frequency is <" << t_point_frequency << ">" << eom );
                //msg_warning( coremsg, "      ratio is <" << t_point_ratio << ">" << eom );
                //msg_warning( coremsg, "      low score is <" << t_update_low_score << ">" << eom );
            }
        }
        for( t_group_it = f_high.begin(); t_group_it != f_high.end(); t_group_it++ )
        {
            for( t_point_it = t_group_it->begin(); t_point_it != t_group_it->end(); t_point_it++ )
            {
                t_point_time = (*t_point_it).time();
                t_point_frequency = (*t_point_it).frequency();
                t_point_ratio = (*t_point_it).ratio();
                t_point_score = (*t_point_it).score();
                t_update_high_score += t_point_score * weight( t_point_frequency - (f_frequency + f_slope * (t_point_time - f_time)) );
                //msg_warning( coremsg, "    high:" << eom );
                //msg_warning( coremsg, "      time is <" << t_point_time << ">" << eom );
                //msg_warning( coremsg, "      frequency is <" << t_point_frequency << ">" << eom );
                //msg_warning( coremsg, "      ratio is <" << t_point_ratio << ">" << eom );
                //msg_warning( coremsg, "      weight is <" << weight( t_point_frequency - (f_frequency + f_slope * (t_point_time - f_time)) ) << ">" << eom );
                //msg_warning( coremsg, "      high score is <" << t_update_high_score << ">" << eom );
            }
        }
        f_score = t_update_high_score + t_update_low_score;

        for( t_point_it = f_points.begin(); t_point_it != f_points.end(); t_point_it++ )
        {
            t_point_time = (*t_point_it).time();
            t_point_frequency = (*t_point_it).frequency();
            t_point_ratio = (*t_point_it).ratio();
            t_point_score = (*t_point_it).score();
            t_update_global_score += t_point_score * weight( t_point_frequency - (f_frequency + f_slope * (t_point_time - f_time)) );
            //msg_warning( coremsg, "    global:" << eom );
            //msg_warning( coremsg, "      time is <" << t_point_time << ">" << eom );
            //msg_warning( coremsg, "      frequency is <" << t_point_frequency << ">" << eom );
            //msg_warning( coremsg, "      ratio is <" << t_point_ratio << ">" << eom );
            //msg_warning( coremsg, "      weight is <" << weight( t_point_frequency - (f_frequency + f_slope * (t_point_time - f_time)) ) << ">" << eom );
            //msg_warning( coremsg, "      global score is <" << t_update_global_score << ">" << eom );
        }
        f_quality = (t_update_high_score + t_update_global_score) * f_correlation * f_correlation;

        //msg_warning( coremsg, "  summary:" << eom );
        //msg_warning( coremsg, "    score is <" << f_score << ">" << eom );
        //msg_warning( coremsg, "    quality is <" << f_quality << ">" << eom );
        //msg_warning( coremsg, "    time is <" << f_time << ">" << eom );
        //msg_warning( coremsg, "    frequency is <" << f_frequency << ">" << eom );
        //msg_warning( coremsg, "    slope is <" << f_slope << ">" << eom );
        //msg_warning( coremsg, "    duration is <" << f_duration << ">" << eom );
        //msg_warning( coremsg, "    deviation is <" << f_deviation << ">" << eom );
        //msg_warning( coremsg, "    correlation is <" << f_correlation << ">" << eom );

        //msg_warning( coremsg, "    low count is <" << f_low_count << ">" << eom );
        //msg_warning( coremsg, "    high count is <" << f_high_count << ">" << eom );
        //msg_warning( coremsg, "    local r sum is <" << f_local_r_sum << ">" << eom );
        //msg_warning( coremsg, "    local rt sum is <" << f_local_rt_sum << ">" << eom );
        //msg_warning( coremsg, "    local rf sum is <" << f_local_rf_sum << ">" << eom );
        //msg_warning( coremsg, "    local rtt sum is <" << f_local_rtt_sum << ">" << eom );
        //msg_warning( coremsg, "    local rff sum is <" << f_local_rff_sum << ">" << eom );
        //msg_warning( coremsg, "    local rtf sum is <" << f_local_rtf_sum << ">" << eom );

        //msg_warning( coremsg, "    global count is <" << f_points_count << ">" << eom );
        //msg_warning( coremsg, "    global r sum is <" << f_points_r_sum << ">" << eom );
        //msg_warning( coremsg, "    global rt sum is <" << f_points_rt_sum << ">" << eom );
        //msg_warning( coremsg, "    global rf sum is <" << f_points_rf_sum << ">" << eom );
        //msg_warning( coremsg, "    global rtt sum is <" << f_points_rtt_sum << ">" << eom );
        //msg_warning( coremsg, "    global rff sum is <" << f_points_rff_sum << ">" << eom );
        //msg_warning( coremsg, "    global rtf sum is <" << f_points_rtf_sum << ">" << eom );

        return;
    }

    void line::finalize()
    {
        //msg_warning( coremsg, "line <" << f_id << "> finalizing:" << eom );

        while( f_low.empty() == false )
        {
            while( f_low.front().empty() == false )
            {
                f_low.front().pop_front();
            }
            f_low.pop_front();
        }
        f_low_count = 0;

        while( f_high.empty() == false )
        {
            while( f_high.front().empty() == false )
            {
                f_points.push_back( f_high.front().front() );
                f_high.front().pop_front();
            }
            f_high.pop_front();
        }
        f_high_count = 0;

        f_count = f_points.size();

        return;
    }

    real_t line::weight( const real_t& p_frequency ) const
    {
        if( p_frequency > 1. * s_width )
        {
            return 0.;
        }
        if( p_frequency < -1. * s_width )
        {
            return 0.;
        }
        return (.5 + .5 * cos( M_PI * p_frequency / s_width ));
    }

}
