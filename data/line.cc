#include "line.hh"
#include "message.hh"

#include <cmath>

namespace midge
{

    count_t line::s_id = 0;
    real_t line::s_window = 0.;
    real_t line::s_width = 0.;
    real_t line::s_trim = 0.;
    real_t line::s_block = 0.;
    count_t line::s_count = 0;
    point_data* line::s_point_data = NULL;

    line::line() :
                f_free( true ),
                f_id( 0 ),
                f_time( 0. ),
                f_frequency( 0. ),
                f_slope( 0. ),
                f_duration( 0. ),
                f_correlation( 0. ),
                f_deviation( 0. ),
                f_score( 0. ),
                f_quality( 0. ),
                f_points(),
                f_cluster(),
                f_cluster_count( 0 ),
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
    line::line( const line& p_copy ) :
                f_free( p_copy.f_free ),
                f_id( p_copy.f_id ),
                f_time( p_copy.f_time ),
                f_frequency( p_copy.f_frequency ),
                f_slope( p_copy.f_slope ),
                f_duration( p_copy.f_duration ),
                f_correlation( p_copy.f_correlation ),
                f_deviation( p_copy.f_deviation ),
                f_score( p_copy.f_score ),
                f_quality( p_copy.f_quality ),
                f_points( p_copy.f_points ),
                f_cluster( p_copy.f_cluster ),
                f_cluster_count( p_copy.f_cluster_count ),
                f_line( p_copy.f_line ),
                f_line_count( p_copy.f_line_count ),
                f_r_sum( p_copy.f_r_sum ),
                f_rt_sum( p_copy.f_rt_sum ),
                f_rf_sum( p_copy.f_rf_sum ),
                f_rtt_sum( p_copy.f_rtt_sum ),
                f_rff_sum( p_copy.f_rff_sum ),
                f_rtf_sum( p_copy.f_rtf_sum )
    {
    }
    line::~line()
    {
    }

    void line::initialize( const count_t& p_index )
    {
        f_free = true;
        f_id = ++s_id;

        //msg_warning( msg, "line <" << f_id << "> initializing:" << eom );

        f_time = s_point_data->points().at( p_index ).time();
        f_frequency = s_point_data->points().at( p_index ).frequency();
        f_slope = 0.;
        f_duration = 0.;
        f_correlation = 0.;
        f_deviation = 0.;
        f_score = 0.;
        f_quality = 0.;
        f_points.clear();

        f_cluster.clear();
        f_cluster_count = 0;
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

    bool line::update()
    {
        //msg_warning( msg, "line <" << f_id << "> updating:" << eom );

        count_t t_index;
        point_it t_point_it;

        count_t t_point_id;
        real_t t_point_time = s_point_data->time_interval() * s_point_data->time_index();
        real_t t_point_duration = t_point_time - f_time;
        real_t t_point_frequency;
        real_t t_point_ratio;
        real_t t_point_score;
        bool_t t_point_free;

        real_t t_update_frequency = f_frequency;
        real_t t_update_slope = f_slope;
        real_t t_update_deviation = f_deviation;
        real_t t_update_correlation = f_correlation;

        point_list t_update_cluster;
        count_t t_update_cluster_count;
        real_t t_update_score;

        real_t t_update_r_sum;
        real_t t_update_rt_sum;
        real_t t_update_rf_sum;
        real_t t_update_rtt_sum;
        real_t t_update_rff_sum;
        real_t t_update_rtf_sum;

        real_t t_update_tt_stat;
        real_t t_update_ff_stat;
        real_t t_update_tf_stat;
        real_t t_update_ttf_stat;
        real_t t_update_dev_stat;

        real_t t_update_center = t_update_frequency + t_update_slope * t_point_duration;
        real_t t_update_min = t_update_center - s_window;
        real_t t_update_max = t_update_center + s_window;

        pair< count_t, count_t > t_current_indices;
        pair< count_t, count_t > t_next_indices;

        t_current_indices.first = (count_t) (floor( t_update_min / s_point_data->frequency_interval() ));
        if( t_current_indices.first < s_point_data->frequency_index() )
        {
            t_current_indices.first = s_point_data->frequency_index();
        }

        t_current_indices.second = (index_t) (ceil( t_update_max / s_point_data->frequency_interval() ));
        if( t_current_indices.second > s_point_data->frequency_index() + s_point_data->size() - 1 )
        {
            t_current_indices.second = s_point_data->frequency_index() + s_point_data->size() - 1;
        }

        while( true )
        {
            //msg_warning( msg, "  accumulating:" << eom );
            //msg_warning( msg, "    update center is <" << t_update_center << ">" << eom );
            //msg_warning( msg, "    update min is <" << t_update_min << ">" << eom );
            //msg_warning( msg, "    update max is <" << t_update_max << ">" << eom );
            //msg_warning( msg, "    current first is <" << t_current_indices.first << ">" << eom );
            //msg_warning( msg, "    current last is <" << t_current_indices.second << ">" << eom );

            if( t_current_indices.first > t_current_indices.second )
            {
                return false;
            }

            t_update_r_sum = f_r_sum;
            t_update_rt_sum = f_rt_sum;
            t_update_rf_sum = f_rf_sum;
            t_update_rtt_sum = f_rtt_sum;
            t_update_rff_sum = f_rff_sum;
            t_update_rtf_sum = f_rtf_sum;

            t_update_cluster.clear();
            t_update_cluster_count = f_cluster_count;
            t_update_score = f_score;

            for( t_index = t_current_indices.first; t_index <= t_current_indices.second; t_index++ )
            {
                point& t_point = s_point_data->points().at( t_index - s_point_data->frequency_index() );
                t_point_id = t_point.id();
                t_point_frequency = t_point.frequency();
                t_point_ratio = t_point.ratio();
                t_point_score = t_point.score();
                t_point_free = t_point.free();

                //msg_warning( msg, "    point:" << eom );
                //msg_warning( msg, "      time is <" << t_point_time << ">" << eom );
                //msg_warning( msg, "      frequency is <" << t_point_frequency << ">" << eom );
                //msg_warning( msg, "      ratio is <" << t_point_ratio << ">" << eom );
                //msg_warning( msg, "      score is <" << t_point_score << ">" << eom );

                if( t_point_frequency < t_update_min )
                {
                    continue;
                }
                if( t_point_frequency > t_update_max )
                {
                    continue;
                }

                t_update_cluster.push_back( t_point );
                t_update_cluster_count += 1;

                if( t_point_free == false )
                {
                    t_update_score -= s_block;
                }
                else
                {
                    t_update_score += t_point_score;

                    if( t_point_score >= 0. )
                    {
                        t_update_r_sum += t_point_ratio;
                        t_update_rt_sum += t_point_ratio * t_point_duration;
                        t_update_rf_sum += t_point_ratio * t_point_frequency;
                        t_update_rtt_sum += t_point_ratio * t_point_duration * t_point_duration;
                        t_update_rff_sum += t_point_ratio * t_point_frequency * t_point_frequency;
                        t_update_rtf_sum += t_point_ratio * t_point_duration * t_point_frequency;
                    }
                }
            }

            //msg_warning( msg, "    update low count is <" << t_update_cluster_low_count << ">" << eom );
            //msg_warning( msg, "    update high count is <" << t_update_cluster_high_count << ">" << eom );
            //msg_warning( msg, "    update r sum is <" << t_update_r_sum << ">" << eom );
            //msg_warning( msg, "    update rt sum is <" << t_update_rt_sum << ">" << eom );
            //msg_warning( msg, "    update rf sum is <" << t_update_rf_sum << ">" << eom );
            //msg_warning( msg, "    update rtt sum is <" << t_update_rtt_sum << ">" << eom );
            //msg_warning( msg, "    update rff sum is <" << t_update_rff_sum << ">" << eom );
            //msg_warning( msg, "    update rtf sum is <" << t_update_rtf_sum << ">" << eom );

            t_update_tt_stat = t_update_r_sum * t_update_rtt_sum - t_update_rt_sum * t_update_rt_sum;
            t_update_ff_stat = t_update_r_sum * t_update_rff_sum - t_update_rf_sum * t_update_rf_sum;
            t_update_tf_stat = t_update_r_sum * t_update_rtf_sum - t_update_rt_sum * t_update_rf_sum;
            t_update_ttf_stat = t_update_rtt_sum * t_update_rf_sum - t_update_rt_sum * t_update_rtf_sum;

            //msg_warning( msg, "    update tt stat is <" << t_update_tt_stat << ">" << eom );
            //msg_warning( msg, "    update ff stat is <" << t_update_ff_stat << ">" << eom );
            //msg_warning( msg, "    update tf stat is <" << t_update_tf_stat << ">" << eom );
            //msg_warning( msg, "    update ttf stat is <" << t_update_ttf_stat << ">" << eom );

            t_update_frequency = t_update_ttf_stat / t_update_tt_stat;
            if( t_update_frequency != t_update_frequency )
            {
                t_update_frequency = t_update_rf_sum / t_update_r_sum;
            }
            //msg_warning( msg, "    update frequency is <" << t_update_frequency << ">" << eom );

            t_update_slope = t_update_tf_stat / t_update_tt_stat;
            if( t_update_slope != t_update_slope )
            {
                t_update_slope = 0.;
            }
            //msg_warning( msg, "    update slope is <" << t_update_slope << ">" << eom );

            t_update_dev_stat = t_update_rff_sum;
            t_update_dev_stat += t_update_slope * t_update_slope * t_update_rtt_sum;
            t_update_dev_stat += t_update_frequency * t_update_frequency * t_update_r_sum;
            t_update_dev_stat -= 2. * t_update_slope * t_update_rtf_sum;
            t_update_dev_stat -= 2. * t_update_frequency * t_update_rf_sum;
            t_update_dev_stat += 2. * t_update_slope * t_update_frequency * t_update_rt_sum;

            t_update_correlation = sqrt( (t_update_tf_stat * t_update_tf_stat) / (t_update_tt_stat * t_update_ff_stat) );
            if( t_update_correlation != t_update_correlation )
            {
                t_update_correlation = 0.;
            }
            else if( t_update_correlation > 1. )
            {
                t_update_correlation = 0.;
            }
            //msg_warning( msg, "    update correlation is <" << t_update_correlation << ">" << eom );

            t_update_deviation = sqrt( t_update_dev_stat / t_update_r_sum );
            if( t_update_deviation != t_update_deviation )
            {
                t_update_deviation = 0.;
            }
            //msg_warning( msg, "    update deviation is <" << t_update_deviation << ">" << eom );

            t_update_center = t_update_frequency + t_update_slope * t_point_duration;
            t_update_min = t_update_center - s_window;
            t_update_max = t_update_center + s_window;

            t_next_indices.first = (count_t) (floor( t_update_min / s_point_data->frequency_interval() ));
            if( t_next_indices.first < s_point_data->frequency_index() )
            {
                t_next_indices.first = s_point_data->frequency_index();
            }

            t_next_indices.second = (index_t) (ceil( t_update_max / s_point_data->frequency_interval() ));
            if( t_next_indices.second > s_point_data->frequency_index() + s_point_data->size() - 1 )
            {
                t_next_indices.second = s_point_data->frequency_index() + s_point_data->size() - 1;
            }

            //msg_warning( msg, "    center now <" << t_update_center << ">" << eom );
            //msg_warning( msg, "    min now <" << t_update_min << ">" << eom );
            //msg_warning( msg, "    max now <" << t_update_max << ">" << eom );
            //msg_warning( msg, "    first now <" << t_next_indices.first << ">" << eom );
            //msg_warning( msg, "    last now <" << t_next_indices.second << ">" << eom );

            if( t_current_indices == t_next_indices )
            {
                for( t_index = t_current_indices.first; t_index <= t_current_indices.second; t_index++ )
                {
                    s_point_data->points().at( t_index - s_point_data->frequency_index() ).free() = false;
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

        f_r_sum = t_update_r_sum;
        f_rt_sum = t_update_rt_sum;
        f_rf_sum = t_update_rf_sum;
        f_rtt_sum = t_update_rtt_sum;
        f_rff_sum = t_update_rff_sum;
        f_rtt_sum = t_update_rtt_sum;
        f_rtf_sum = t_update_rtf_sum;

        f_cluster.push_back( t_update_cluster );
        f_cluster_count = t_update_cluster_count;
        f_score = t_update_score;

        //msg_warning( msg, "  trimming:" << eom );
        if( (f_cluster_count - f_cluster.front().size()) >= s_count )
        {
            for( t_point_it = f_cluster.front().begin(); t_point_it != f_cluster.front().end(); t_point_it++ )
            {
                f_cluster_count -= 1;
                if( t_point_it->free() == true )
                {
                    f_score -= t_point_it->score();
                }
                else
                {
                    f_score += s_block;
                }
            }
            f_line.push_back( f_cluster.front() );
            f_cluster.pop_front();
        }

        //msg_warning( msg, "  summary:" << eom );
        //msg_warning( msg, "    score is <" << f_score << ">" << eom );
        //msg_warning( msg, "    time is <" << f_time << ">" << eom );
        //msg_warning( msg, "    frequency is <" << f_frequency << ">" << eom );
        //msg_warning( msg, "    slope is <" << f_slope << ">" << eom );
        //msg_warning( msg, "    duration is <" << f_duration << ">" << eom );
        //msg_warning( msg, "    deviation is <" << f_deviation << ">" << eom );
        //msg_warning( msg, "    correlation is <" << f_correlation << ">" << eom );

        //msg_warning( msg, "    cluster low count is <" << f_cluster_low_count << ">" << eom );
        //msg_warning( msg, "    cluster high count is <" << f_cluster_high_count << ">" << eom );
        //msg_warning( msg, "    line count is <" << f_line_count << ">" << eom );
        //msg_warning( msg, "    r sum is <" << f_r_sum << ">" << eom );
        //msg_warning( msg, "    rt sum is <" << f_rt_sum << ">" << eom );
        //msg_warning( msg, "    rf sum is <" << f_rf_sum << ">" << eom );
        //msg_warning( msg, "    rtt sum is <" << f_rtt_sum << ">" << eom );
        //msg_warning( msg, "    rff sum is <" << f_rff_sum << ">" << eom );
        //msg_warning( msg, "    rtf sum is <" << f_rtf_sum << ">" << eom );

        return true;
    }

    void line::finalize()
    {
        group_it t_group_it;
        point_it t_point_it;

        real_t t_point_time;
        real_t t_point_frequency;
        real_t t_point_offset;
        real_t t_point_score;
        real_t t_point_weight;

        real_t t_update_w_sum;
        real_t t_update_ws_sum;
        group_it t_update_it;

        while( f_cluster.empty() == false )
        {
            f_line.push_back( f_cluster.front() );
            f_cluster.pop_front();
        }

        t_update_w_sum = 0.;
        t_update_ws_sum = 0.;
        for( t_group_it = f_line.begin(); t_group_it != f_line.end(); t_group_it++ )
        {
            for( t_point_it = t_group_it->begin(); t_point_it != t_group_it->end(); t_point_it++ )
            {
                t_point_time = t_point_it->time();
                t_point_frequency = t_point_it->frequency();
                t_point_offset = t_point_frequency - (f_frequency + f_slope * (t_point_time - f_time));

                if( t_point_offset > s_width )
                {
                    continue;
                }
                if( t_point_offset < -s_width )
                {
                    continue;
                }

                t_point_weight = .5 + .5 * cos( M_PI * t_point_offset / s_width );
                t_point_score = t_point_it->score();

                t_update_w_sum += t_point_weight;
                t_update_ws_sum += t_point_weight * t_point_score;
            }
        }

        f_score = t_update_ws_sum / t_update_w_sum;
        f_quality = t_update_ws_sum * f_correlation;

        t_update_it = f_line.end();
        while( t_update_it != f_line.begin() )
        {
            --t_update_it;

            t_update_w_sum = 0.;
            t_update_ws_sum = 0.;

            for( t_point_it = t_update_it->begin(); t_point_it != t_update_it->end(); t_point_it++ )
            {
                t_point_time = t_point_it->time();
                t_point_frequency = t_point_it->frequency();
                t_point_offset = t_point_frequency - (f_frequency + f_slope * (t_point_time - f_time));

                if( t_point_offset > s_width )
                {
                    continue;
                }
                if( t_point_offset < -s_width )
                {
                    continue;
                }

                t_point_weight = .5 + .5 * cos( M_PI * t_point_offset / s_width );
                t_point_score = t_point_it->score();

                t_update_w_sum += t_point_weight;
                t_update_ws_sum += t_point_weight * t_point_score;
            }

            f_duration = t_point_time - f_time;

            if( t_update_ws_sum / t_update_w_sum > f_score * s_trim )
            {
                break;
            }
        }
        ++t_update_it;

        for( t_group_it = f_line.begin(); t_group_it != t_update_it; t_group_it++ )
        {
            for( t_point_it = t_group_it->begin(); t_point_it != t_group_it->end(); t_point_it++ )
            {
                t_point_time = t_point_it->time();
                t_point_frequency = t_point_it->frequency();
                t_point_offset = t_point_frequency - (f_frequency + f_slope * (t_point_time - f_time));

                if( t_point_offset > s_width )
                {
                    continue;
                }
                if( t_point_offset < -s_width )
                {
                    continue;
                }

                f_points.push_back( *t_point_it );
            }
        }

        while( f_line.empty() == false )
        {
            f_line.pop_front();
        }

        return;
    }

}
