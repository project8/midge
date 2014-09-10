#include "line.hh"
#include "coremsg.hh"

#include <cmath>

namespace midge
{

    count_t line::s_id = 0;
    real_t line::s_threshold = 0.;
    real_t line::s_window = 0.;
    real_t line::s_tolerance = 0.;
    count_t line::s_count = 0;
    real_t line::s_hit_factor = 0.;
    real_t line::s_hit_power = 0.;
    real_t line::s_miss_factor = 0.;
    real_t line::s_miss_power = 0.;

    point_data* line::s_data = NULL;

    line::line( const count_t& p_index ) :
            f_id( ++s_id ),
            f_score( 0. ),
            f_time( s_data->points().at( p_index )->time() ),
            f_frequency( s_data->points().at( p_index )->frequency() ),
            f_slope( 0. ),
            f_duration( 0. ),
            f_correlation( 0. ),
            f_deviation( 0. ),
            f_low(),
            f_low_count( 0 ),
            f_high(),
            f_high_count( 0 ),
            f_local_r_sum( 0. ),
            f_local_rt_sum( 0. ),
            f_local_rf_sum( 0. ),
            f_local_rtt_sum( 0. ),
            f_local_rff_sum( 0. ),
            f_local_rtf_sum( 0. ),
            f_global(),
            f_global_count( 0 ),
            f_global_r_sum( 0. ),
            f_global_rt_sum( 0. ),
            f_global_rf_sum( 0. ),
            f_global_rtt_sum( 0. ),
            f_global_rff_sum( 0. ),
            f_global_rtf_sum( 0. )
    {
    }
    line::~line()
    {
    }

    void line::update()
    {
        //msg_warning( coremsg, "line <" << f_id << "> updating:" << eom );

        point_it t_point_it;
        group_it t_group_it;

        register count_t t_point_id;
        register real_t t_point_time = s_data->get_time_interval() * s_data->get_time_index();
        register real_t t_point_duration = t_point_time - f_time;
        register real_t t_point_frequency;
        register real_t t_point_ratio;

        register real_t t_update_hit_score = 0.;
        register real_t t_update_miss_score = 0.;
        register real_t t_update_current_miss_score = 0.;
        register real_t t_update_current_miss = 0.;
        register real_t t_update_frequency = f_frequency;
        register real_t t_update_slope = f_slope;
        register real_t t_update_deviation = f_deviation;
        register real_t t_update_correlation = f_correlation;
        register real_t t_update_center = t_update_frequency + t_update_slope * t_point_duration;
        register real_t t_update_min = t_update_center - s_window;
        register real_t t_update_max = t_update_center + s_window;

        register real_t t_update_tt_stat;
        register real_t t_update_ff_stat;
        register real_t t_update_tf_stat;
        register real_t t_update_ttf_stat;
        register real_t t_update_dev_stat;

        register count_t t_index;
        register pair< count_t, count_t > t_current_indices;
        register pair< count_t, count_t > t_next_indices;

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

        pointer< point > t_loop_point;

        list< pointer< point > > t_loop_low;
        register count_t t_loop_low_count;

        list< pointer< point > > t_loop_high;
        register count_t t_loop_high_count;
        register real_t t_loop_high_r_sum;
        register real_t t_loop_high_rt_sum;
        register real_t t_loop_high_rf_sum;
        register real_t t_loop_high_rtt_sum;
        register real_t t_loop_high_rff_sum;
        register real_t t_loop_high_rtf_sum;

        register count_t t_loop_total_count;
        register real_t t_loop_total_r_sum;
        register real_t t_loop_total_rt_sum;
        register real_t t_loop_total_rf_sum;
        register real_t t_loop_total_rtt_sum;
        register real_t t_loop_total_rff_sum;
        register real_t t_loop_total_rtf_sum;

        while( true )
        {
            //msg_warning( coremsg, "  accumulating:" << eom );
            //msg_warning( coremsg, "    center is <" << t_update_center << ">" << eom );
            //msg_warning( coremsg, "    min is <" << t_update_min << ">" << eom );
            //msg_warning( coremsg, "    max is <" << t_update_max << ">" << eom );
            //msg_warning( coremsg, "    first is <" << t_current_indices.first << ">" << eom );
            //msg_warning( coremsg, "    last is <" << t_current_indices.second << ">" << eom );

            t_loop_point = NULL;

            t_loop_low.clear();
            t_loop_low_count = f_low_count;

            t_loop_high.clear();
            t_loop_high_count = f_high_count;
            t_loop_high_r_sum = f_local_r_sum;
            t_loop_high_rt_sum = f_local_rt_sum;
            t_loop_high_rf_sum = f_local_rf_sum;
            t_loop_high_rtt_sum = f_local_rtt_sum;
            t_loop_high_rff_sum = f_local_rff_sum;
            t_loop_high_rtf_sum = f_local_rtf_sum;

            t_loop_total_count = f_global_count;
            t_loop_total_r_sum = f_global_r_sum;
            t_loop_total_rt_sum = f_global_rt_sum;
            t_loop_total_rf_sum = f_global_rf_sum;
            t_loop_total_rtt_sum = f_global_rtt_sum;
            t_loop_total_rff_sum = f_global_rff_sum;
            t_loop_total_rtf_sum = f_global_rtf_sum;

            for( t_index = t_current_indices.first; t_index <= t_current_indices.second; t_index++ )
            {
                t_loop_point = s_data->points().at( t_index - s_data->get_frequency_index() );
                t_point_id = t_loop_point->id();
                t_point_frequency = t_loop_point->frequency();
                t_point_ratio = t_loop_point->ratio();

                //msg_warning( coremsg, "    point:" << eom );
                //msg_warning( coremsg, "      time is <" << t_point_time << ">" << eom );
                //msg_warning( coremsg, "      frequency is <" << t_point_frequency << ">" << eom );
                //msg_warning( coremsg, "      ratio is <" << t_point_ratio << ">" << eom );

                if( t_point_id > 0 )
                {
                    continue;
                }
                if( t_point_frequency < t_update_min )
                {
                    continue;
                }
                if( t_point_frequency > t_update_max )
                {
                    continue;
                }

                if( t_point_ratio < s_threshold )
                {
                    //msg_warning( coremsg, "      point low" << eom );
                    t_loop_low_count += 1;
                    t_loop_low.push_back( t_loop_point );
                }
                else
                {
                    //msg_warning( coremsg, "      point high" << eom );
                    t_loop_high_count += 1;
                    t_loop_high_r_sum += t_point_ratio;
                    t_loop_high_rt_sum += t_point_ratio * t_point_duration;
                    t_loop_high_rf_sum += t_point_ratio * t_point_frequency;
                    t_loop_high_rtt_sum += t_point_ratio * t_point_duration * t_point_duration;
                    t_loop_high_rff_sum += t_point_ratio * t_point_frequency * t_point_frequency;
                    t_loop_high_rtf_sum += t_point_ratio * t_point_duration * t_point_frequency;
                    t_loop_high.push_back( t_loop_point );
                }
            }

            t_loop_total_count += t_loop_high_count;
            t_loop_total_r_sum += t_loop_high_r_sum;
            t_loop_total_rt_sum += t_loop_high_rt_sum;
            t_loop_total_rf_sum += t_loop_high_rf_sum;
            t_loop_total_rtt_sum += t_loop_high_rtt_sum;
            t_loop_total_rff_sum += t_loop_high_rff_sum;
            t_loop_total_rtf_sum += t_loop_high_rtf_sum;

            if( t_loop_high.size() == 0 )
            {
                for( t_point_it = t_loop_low.begin(); t_point_it != t_loop_low.end(); t_point_it++ )
                {
                    (*t_point_it)->id() = f_id;
                }
                break;
            }

            //msg_warning( coremsg, "    total count is <" << t_loop_total_count << ">" << eom );
            //msg_warning( coremsg, "    total r sum is <" << t_loop_total_r_sum << ">" << eom );
            //msg_warning( coremsg, "    total rt sum is <" << t_loop_total_rt_sum << ">" << eom );
            //msg_warning( coremsg, "    total rf sum is <" << t_loop_total_rf_sum << ">" << eom );
            //msg_warning( coremsg, "    total rtt sum is <" << t_loop_total_rtt_sum << ">" << eom );
            //msg_warning( coremsg, "    total rff sum is <" << t_loop_total_rff_sum << ">" << eom );
            //msg_warning( coremsg, "    total rtf sum is <" << t_loop_total_rtf_sum << ">" << eom );

            t_update_tt_stat = t_loop_total_r_sum * t_loop_total_rtt_sum - t_loop_total_rt_sum * t_loop_total_rt_sum;
            t_update_ff_stat = t_loop_total_r_sum * t_loop_total_rff_sum - t_loop_total_rf_sum * t_loop_total_rf_sum;
            t_update_tf_stat = t_loop_total_r_sum * t_loop_total_rtf_sum - t_loop_total_rt_sum * t_loop_total_rf_sum;
            t_update_ttf_stat = t_loop_total_rtt_sum * t_loop_total_rf_sum - t_loop_total_rt_sum * t_loop_total_rtf_sum;

            //msg_warning( coremsg, "    update tt stat is <" << t_update_tt_stat << ">" << eom );
            //msg_warning( coremsg, "    update ff stat is <" << t_update_ff_stat << ">" << eom );
            //msg_warning( coremsg, "    update tf stat is <" << t_update_tf_stat << ">" << eom );
            //msg_warning( coremsg, "    update ttf stat is <" << t_update_ttf_stat << ">" << eom );

            t_update_frequency = t_update_ttf_stat / t_update_tt_stat;
            if( t_update_frequency != t_update_frequency )
            {
                t_update_frequency = t_loop_total_rf_sum / t_loop_total_r_sum;
            }
            //msg_warning( coremsg, "    frequency is <" << t_update_frequency << ">" << eom );

            t_update_slope = t_update_tf_stat / t_update_tt_stat;
            if( t_update_slope != t_update_slope )
            {
                t_update_slope = 0.;
            }
            //msg_warning( coremsg, "    slope is <" << t_update_slope << ">" << eom );

            t_update_dev_stat = t_loop_total_rff_sum;
            t_update_dev_stat += t_update_slope * t_update_slope * t_loop_total_rtt_sum;
            t_update_dev_stat += t_update_frequency * t_update_frequency * t_loop_total_r_sum;
            t_update_dev_stat -= 2. * t_update_slope * t_loop_total_rtf_sum;
            t_update_dev_stat -= 2. * t_update_frequency * t_loop_total_rf_sum;
            t_update_dev_stat += 2. * t_update_slope * t_update_frequency * t_loop_total_rt_sum;

            t_update_correlation = sqrt( (t_update_tf_stat * t_update_tf_stat) / (t_update_tt_stat * t_update_ff_stat) );
            if( t_update_correlation != t_update_correlation )
            {
                t_update_correlation = 0.;
            }
            //msg_warning( coremsg, "    correlation is <" << t_update_correlation << ">" << eom );

            t_update_deviation = sqrt( t_update_dev_stat / t_loop_total_r_sum );
            if( t_update_deviation != t_update_deviation )
            {
                //msg_warning( coremsg, "    false deviation is <" << t_update_deviation << ">" << eom );
                t_update_deviation = s_window / (2. * s_tolerance);
            }
            else
            {
                //msg_warning( coremsg, "    true deviation is <" << t_update_deviation << ">" << eom );
            }

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
                for( t_point_it = t_loop_low.begin(); t_point_it != t_loop_low.end(); t_point_it++ )
                {
                    (*t_point_it)->id() = f_id;
                }
                for( t_point_it = t_loop_high.begin(); t_point_it != t_loop_high.end(); t_point_it++ )
                {
                    (*t_point_it)->id() = f_id;
                }
                break;
            }
            else
            {
                t_current_indices = t_next_indices;
            }
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

        f_local_r_sum = t_loop_high_r_sum;
        f_local_rt_sum = t_loop_high_rt_sum;
        f_local_rf_sum = t_loop_high_rf_sum;
        f_local_rtt_sum = t_loop_high_rtt_sum;
        f_local_rff_sum = t_loop_high_rff_sum;
        f_local_rtt_sum = t_loop_high_rtt_sum;
        f_local_rtf_sum = t_loop_high_rtf_sum;

        //msg_warning( coremsg, "  trimming:" << eom );
        if( (f_low_count + f_high_count - f_low.front().size() - f_high.front().size()) >= s_count )
        {
            while( f_low.front().empty() == false )
            {
                t_loop_point = f_low.front().front();
                t_point_time = t_loop_point->time();
                t_point_frequency = t_loop_point->frequency();
                t_point_ratio = t_loop_point->ratio();

                //msg_warning( coremsg, "    low point:" << eom );
                //msg_warning( coremsg, "      time is <" << t_point_time << ">" << eom );
                //msg_warning( coremsg, "      frequency is <" << t_point_frequency << ">" << eom );
                //msg_warning( coremsg, "      ratio is <" << t_point_ratio << ">" << eom );

                f_low_count -= 1;

                //msg_warning( coremsg, "      point freed" << eom );
                pool< point >::free( t_loop_point );

                f_low.front().pop_front();
            }
            f_low.pop_front();

            while( f_high.front().empty() == false )
            {
                t_loop_point = f_high.front().front();
                t_point_time = t_loop_point->time();
                t_point_frequency = t_loop_point->frequency();
                t_point_ratio = t_loop_point->ratio();

                //msg_warning( coremsg, "    high point:" << eom );
                //msg_warning( coremsg, "      time is <" << t_point_time << ">" << eom );
                //msg_warning( coremsg, "      frequency is <" << t_point_frequency << ">" << eom );
                //msg_warning( coremsg, "      ratio is <" << t_point_ratio << ">" << eom );

                f_high_count -= 1;
                f_local_r_sum -= t_point_ratio;
                f_local_rt_sum -= t_point_ratio * t_point_time;
                f_local_rf_sum -= t_point_ratio * t_point_frequency;
                f_local_rtt_sum -= t_point_ratio * t_point_time * t_point_time;
                f_local_rff_sum -= t_point_ratio * t_point_frequency * t_point_frequency;
                f_local_rtf_sum -= t_point_ratio * t_point_time * t_point_frequency;

                //msg_warning( coremsg, "      point retained" << eom );
                f_global.push_back( t_loop_point );
                f_global_count += 1;
                f_global_r_sum += t_point_ratio;
                f_global_rt_sum += t_point_ratio * t_point_time;
                f_global_rf_sum += t_point_ratio * t_point_frequency;
                f_global_rtt_sum += t_point_ratio * t_point_time * t_point_time;
                f_global_rff_sum += t_point_ratio * t_point_frequency * t_point_frequency;
                f_global_rtf_sum += t_point_ratio * t_point_time * t_point_frequency;

                f_high.front().pop_front();
            }
            f_high.pop_front();
        }

        //msg_warning( coremsg, "  updating:" << eom );
        for( t_group_it = f_high.begin(); t_group_it != f_high.end(); t_group_it++ )
        {
            if( t_group_it->size() == 0 )
            {
                t_update_current_miss += 1.;
                t_update_current_miss_score = s_miss_factor * pow( t_update_current_miss, s_miss_power );
                //msg_warning( coremsg, "    missed:" << eom );
                //msg_warning( coremsg, "      hit score is <" << t_update_hit_score << ">" << eom );
                //msg_warning( coremsg, "      miss score is <" << t_update_miss_score << ">" << eom );
                //msg_warning( coremsg, "      current miss score is <" << t_update_current_miss_score << ">" << eom );
            }
            else
            {
                t_update_miss_score += t_update_current_miss_score;
                t_update_current_miss_score = 0.;
                t_update_current_miss = 0.;
                for( t_point_it = t_group_it->begin(); t_point_it != t_group_it->end(); t_point_it++ )
                {
                    t_point_frequency = (*t_point_it)->frequency();
                    t_point_ratio = (*t_point_it)->ratio();
                    t_update_hit_score += s_hit_factor * pow( t_point_ratio - s_threshold + 1., s_hit_power ) * weight( t_point_frequency - t_update_center );
                    //msg_warning( coremsg, "    hit:" << eom );
                    //msg_warning( coremsg, "      frequency is <" << t_point_frequency << ">" << eom );
                    //msg_warning( coremsg, "      ratio is <" << t_point_ratio << ">" << eom );
                    //msg_warning( coremsg, "      weight is <" << weight( t_point_frequency - t_update_center ) << ">" << eom );
                    //msg_warning( coremsg, "      hit score is <" << t_update_hit_score << ">" << eom );
                    //msg_warning( coremsg, "      miss score is <" << t_update_miss_score << ">" << eom );
                    //msg_warning( coremsg, "      current miss score is <" << t_update_current_miss_score << ">" << eom );
                }
            }
        }
        f_score = t_update_hit_score - t_update_miss_score - t_update_current_miss_score;

        //msg_warning( coremsg, "  summary:" << eom );
        //msg_warning( coremsg, "    low count is <" << f_low_count << ">" << eom );
        //msg_warning( coremsg, "    high count is <" << f_high_count << ">" << eom );
        //msg_warning( coremsg, "    local r sum is <" << f_local_r_sum << ">" << eom );
        //msg_warning( coremsg, "    local rt sum is <" << f_local_rt_sum << ">" << eom );
        //msg_warning( coremsg, "    local rf sum is <" << f_local_rf_sum << ">" << eom );
        //msg_warning( coremsg, "    local rtt sum is <" << f_local_rtt_sum << ">" << eom );
        //msg_warning( coremsg, "    local rff sum is <" << f_local_rff_sum << ">" << eom );
        //msg_warning( coremsg, "    local rtf sum is <" << f_local_rtf_sum << ">" << eom );

        //msg_warning( coremsg, "    global count is <" << f_global_count << ">" << eom );
        //msg_warning( coremsg, "    global r sum is <" << f_global_r_sum << ">" << eom );
        //msg_warning( coremsg, "    global rt sum is <" << f_global_rt_sum << ">" << eom );
        //msg_warning( coremsg, "    global rf sum is <" << f_global_rf_sum << ">" << eom );
        //msg_warning( coremsg, "    global rtt sum is <" << f_global_rtt_sum << ">" << eom );
        //msg_warning( coremsg, "    global rff sum is <" << f_global_rff_sum << ">" << eom );
        //msg_warning( coremsg, "    global rtf sum is <" << f_global_rtf_sum << ">" << eom );

        //msg_warning( coremsg, "    score is <" << f_score << ">" << eom );
        //msg_warning( coremsg, "    time is <" << f_time << ">" << eom );
        //msg_warning( coremsg, "    frequency is <" << f_frequency << ">" << eom );
        //msg_warning( coremsg, "    slope is <" << f_slope << ">" << eom );
        //msg_warning( coremsg, "    duration is <" << f_duration << ">" << eom );
        //msg_warning( coremsg, "    deviation is <" << f_deviation << ">" << eom );
        //msg_warning( coremsg, "    correlation is <" << f_correlation << ">" << eom );

        return;
    }

    real_t line::weight( const real_t& p_frequency ) const
    {
        if( p_frequency > 1. * s_window )
        {
            return 0.;
        }
        if( p_frequency < -1. * s_window )
        {
            return 0.;
        }
        return (.5 + .5 * cos( M_PI * p_frequency / s_window ));
    }

}
