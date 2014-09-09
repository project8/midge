#include "line_transformer.hh"

#include <limits>
using std::numeric_limits;

#include <cmath>

namespace midge
{

    line_transformer::line_transformer() :
            f_ratio_seed( 1. ),
            f_ratio_low( 0. ),
            f_ratio_high( 10. ),
            f_window_low( 10.e-3 ),
            f_window_seed( 30.e-3 ),
            f_window_high( 30.e-3 ),
            f_size( 8 ),
            f_tolerance( 1. ),
            f_length( 10 )
    {
    }

    line_transformer::~line_transformer()
    {
    }

    void line_transformer::initialize()
    {
        out_buffer< 0 >().initialize( f_length );
        return;
    }

    void line_transformer::execute()
    {
        line_list t_candidate_lines;
        line_list t_confirmed_lines;
        line_list t_complete_lines;

        line_stack t_line_complete_stack;
        line_stack t_line_confirm_stack;
        line_stack t_line_discard_stack;

        count_t t_index;
        command_t t_command;

        candidate_data* t_candidates;
        line_data* t_lines;

        count_t t_line_id = 0;
        while( true )
        {
            t_command = in_stream< 0 >().get();
            t_candidates = in_stream< 0 >().data();
            t_lines = out_stream< 0 >().data();

            if( t_command == stream::s_start )
            {
                t_lines->set_size( t_candidates->candidates().size() );
                t_lines->set_time_interval( t_candidates->get_time_interval() );
                t_lines->set_time_index( t_candidates->get_time_index() );
                t_lines->set_frequency_interval( t_candidates->get_frequency_interval() );
                t_lines->set_frequency_index( t_candidates->get_frequency_index() );

                out_stream< 0 >().set( stream::s_start );
                continue;
            }

            if( t_command == stream::s_run )
            {
                t_lines->set_size( t_candidates->candidates().size() );
                t_lines->set_time_interval( t_candidates->get_time_interval() );
                t_lines->set_time_index( t_candidates->get_time_index() );
                t_lines->set_frequency_interval( t_candidates->get_frequency_interval() );
                t_lines->set_frequency_index( t_candidates->get_frequency_index() );

                line_it t_line_it;
                pointer< line > t_line;
                for( t_line_it = t_confirmed_lines.begin(); t_line_it != t_confirmed_lines.end(); t_line_it++ )
                {
                    t_line = *t_line_it;

                    msg_warning( coremsg, "**   updating confirmed line <" << t_line->id() << "> **" << eom );
                    update_line( *t_candidates, *t_line );

                    if( t_line->ratio() < f_ratio_low )
                    {
                        msg_warning( coremsg, "**   will complete confirmed line <" << t_line->id() << "> **" << eom );
                        t_line_complete_stack.push( t_line_it );
                    }
                }
                for( t_line_it = t_candidate_lines.begin(); t_line_it != t_candidate_lines.end(); t_line_it++ )
                {
                    t_line = *t_line_it;

                    msg_warning( coremsg, "**   updating candidate line <" << t_line->id() << "> **" << eom );
                    update_line( *t_candidates, *t_line );

                    if( t_line->ratio() > f_ratio_high )
                    {
                        msg_warning( coremsg, "**   will confirm candidate line <" << t_line->id() << "> **" << eom );
                        t_line_confirm_stack.push( t_line_it );
                    }
                    if( t_line->ratio() < f_ratio_low )
                    {
                        msg_warning( coremsg, "**   will discard candidate line <" << t_line->id() << "> **" << eom );
                        t_line_discard_stack.push( t_line_it );
                    }
                }
                for( count_t t_index = 0; t_index <= t_candidates->candidates().size(); t_index++ )
                {
                    if( t_candidates->candidates().at( t_index )->ratio() > f_ratio_seed )
                    {
                        t_line = new line();
                        t_line->id() = ++t_line_id;
                        t_line->time() = t_candidates->candidates().at( t_index )->time();
                        t_line->frequency() = t_candidates->candidates().at( t_index )->frequency();
                        t_line->ratio() = f_ratio_seed;
                        t_line->window() = f_window_seed;

                        t_line_it = t_candidate_lines.insert( t_candidate_lines.end(), t_line );

                        msg_warning( coremsg, "**   initializing created line <" << t_line->id() << "> **" << eom );
                        initialize_line( *t_candidates, *t_line );

                        if( t_line->ratio() < f_ratio_low )
                        {
                            msg_warning( coremsg, "**   will discard created line <" << t_line->id() << "> **" << eom );
                            t_line_discard_stack.push( t_line_it );
                        }
                    }
                }

                msg_warning( coremsg, "** completing lines **" << eom );
                while( t_line_complete_stack.empty() == false )
                {
                    t_line = *(t_line_complete_stack.top());
                    t_complete_lines.push_back( t_line );
                    msg_warning( coremsg, "**   completing line <" << t_line->id() << "> **" << eom );

                    t_confirmed_lines.erase( t_line_complete_stack.top() );
                    t_line_complete_stack.pop();
                }

                msg_warning( coremsg, "** confirming lines **" << eom );
                while( t_line_confirm_stack.empty() == false )
                {
                    t_line = *(t_line_confirm_stack.top());
                    t_confirmed_lines.push_back( t_line );
                    msg_warning( coremsg, "**   confirming line <" << t_line->id() << "> **" << eom );

                    t_candidate_lines.erase( t_line_confirm_stack.top() );
                    t_line_confirm_stack.pop();
                }

                msg_warning( coremsg, "** discarding lines **" << eom );
                while( t_line_discard_stack.empty() == false )
                {
                    t_line = *(t_line_discard_stack.top());
                    msg_warning( coremsg, "**   discarding line <" << t_line->id() << "> **" << eom );

                    t_candidate_lines.erase( t_line_discard_stack.top() );
                    t_line_discard_stack.pop();

                    delete t_line;
                }

                out_stream< 0 >().set( stream::s_run );
                continue;
            }

            if( t_command == stream::s_stop )
            {
                out_stream< 0 >().set( stream::s_stop );
                continue;
            }

            if( t_command == stream::s_exit )
            {
                out_stream< 0 >().set( stream::s_exit );
                return;
            }
        }
    }

    void line_transformer::finalize()
    {
        out_buffer< 0 >().finalize();
        return;
    }

    void line_transformer::initialize_line( candidate_data& p_candidates, line& p_line )
    {
        msg_warning( coremsg, "line <" << p_line.id() << "> initializing:" << eom );

        register count_t t_index;
        register count_t t_point_id;
        register real_t t_point_time = p_line.time();
        register real_t t_point_frequency;
        register real_t t_point_ratio;

        register real_t t_init_frequency = p_line.frequency();
        register real_t t_init_min = t_init_frequency - p_line.window();
        register real_t t_init_max = t_init_frequency + p_line.window();
        register real_t t_init_dev_stat;

        pair< count_t, count_t > t_current_indices;

        t_current_indices.first = (count_t) (floor( (t_init_min / p_candidates.get_frequency_interval()) - p_candidates.get_frequency_index() ));
        if( t_current_indices.first < 0 )
        {
            t_current_indices.first = 0;
        }

        t_current_indices.second = (count_t) (ceil( (t_init_max / p_candidates.get_frequency_interval()) - p_candidates.get_frequency_index() ));
        if( t_current_indices.second > p_candidates.candidates().size() - 1 )
        {
            t_current_indices.second = p_candidates.candidates().size() - 1;
        }

        pair< count_t, count_t > t_next_indices;

        pointer< candidate > t_loop_point;
        list< pointer< candidate > > t_loop_points;
        register real_t t_loop_sum;
        register real_t t_loop_r_sum;
        register real_t t_loop_rf_sum;
        register real_t t_loop_rff_sum;
        while( true )
        {
            msg_warning( coremsg, "  calculating properties:" << eom );
            msg_warning( coremsg, "      frequency is <" << t_init_frequency << ">" << eom );
            msg_warning( coremsg, "      min is <" << t_init_min << ">" << eom );
            msg_warning( coremsg, "      max is <" << t_init_max << ">" << eom );

            t_loop_point = NULL;
            t_loop_points.clear();
            t_loop_sum = 0.;
            t_loop_r_sum = 0.;
            t_loop_rf_sum = 0.;
            t_loop_rff_sum = 0.;
            for( t_index = t_current_indices.first; t_index <= t_current_indices.second; t_index++ )
            {
                t_loop_point = p_candidates.candidates().at( t_index );
                t_point_id = t_loop_point->id();
                t_point_frequency = t_loop_point->frequency();
                t_point_ratio = t_loop_point->ratio();

                if( t_point_id > 0 )
                {
                    continue;
                }
                if( t_point_frequency < t_init_min )
                {
                    continue;
                }
                if( t_point_frequency > t_init_max )
                {
                    continue;
                }

                msg_warning( coremsg, "    point:" << eom );
                msg_warning( coremsg, "      time is <" << t_point_time << ">" << eom );
                msg_warning( coremsg, "      frequency is <" << t_point_frequency << ">" << eom );
                msg_warning( coremsg, "      ratio is <" << t_point_ratio << ">" << eom );
                t_loop_points.push_back( t_loop_point );
                t_loop_sum += 1.;
                t_loop_r_sum += t_point_ratio;
                t_loop_rf_sum += t_point_ratio * t_point_frequency;
                t_loop_rff_sum += t_point_ratio * t_point_frequency * t_point_frequency;
            }

            t_init_frequency = t_loop_rf_sum / t_loop_r_sum;
            t_init_min = t_init_frequency - p_line.window();
            t_init_max = t_init_frequency + p_line.window();

            t_next_indices.first = (count_t) (floor( (t_init_min / p_candidates.get_frequency_interval()) - p_candidates.get_frequency_index() ));
            if( t_next_indices.first < 0 )
            {
                t_next_indices.first = 0;
            }

            t_next_indices.second = (count_t) (ceil( (t_init_max / p_candidates.get_frequency_interval()) - p_candidates.get_frequency_index() ));
            if( t_next_indices.second > p_candidates.candidates().size() - 1 )
            {
                t_next_indices.second = p_candidates.candidates().size() - 1;
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

        p_line.local().push_back( t_loop_points );
        p_line.local_sum() = t_loop_sum;
        p_line.local_r_sum() = t_loop_r_sum;
        p_line.local_rf_sum() = t_loop_rf_sum;
        p_line.local_rff_sum() = t_loop_rff_sum;

        p_line.frequency() = t_init_frequency;

        t_init_dev_stat = t_loop_rff_sum;
        t_init_dev_stat += t_init_frequency * t_init_frequency * t_loop_r_sum;
        t_init_dev_stat -= 2. * t_init_frequency * t_loop_rf_sum;
        if( t_init_dev_stat > 0. )
        {
            p_line.deviation() = sqrt( t_init_dev_stat / t_loop_r_sum );
        }
        else
        {
            p_line.deviation() = 0.;
        }

        if( p_line.sum() > f_size )
        {
            p_line.ratio() = p_line.r_sum() / p_line.sum();
            p_line.window() = f_tolerance * p_line.deviation();
            if( p_line.window() < f_window_low )
            {
                p_line.window() = f_window_low;
            }
            if( p_line.window() > f_window_high )
            {
                p_line.window() = f_window_high;
            }
        }
        else
        {
            p_line.ratio() = f_ratio_seed;
            p_line.window() = f_window_seed;
        }

        msg_warning( coremsg, "    frequency is <" << p_line.frequency() << ">" << eom );
        msg_warning( coremsg, "    slope is <" << p_line.slope() << ">" << eom );
        msg_warning( coremsg, "    deviation is <" << p_line.deviation() << ">" << eom );
        msg_warning( coremsg, "    correlation is <" << p_line.correlation() << ">" << eom );
        msg_warning( coremsg, "    ratio is <" << p_line.ratio() << ">" << eom );
        msg_warning( coremsg, "    window is <" << p_line.window() << ">" << eom );

        return;
    }

    void line_transformer::update_line( candidate_data& p_candidates, line& p_line )
    {
        msg_warning( coremsg, "line <" << p_line.id() << "> updating:" << eom );

        register count_t t_index;
        register count_t t_point_id;
        register real_t t_point_time = p_candidates.get_time_interval() * p_candidates.get_time_index();
        register real_t t_point_duration = t_point_time - p_line.time();
        register real_t t_point_frequency;
        register real_t t_point_ratio;

        register real_t t_update_frequency = p_line.frequency();
        register real_t t_update_slope = p_line.slope();
        register real_t t_update_center = t_update_frequency + t_update_slope * t_point_duration;
        register real_t t_update_min = t_update_center - p_line.window();
        register real_t t_update_max = t_update_center + p_line.window();

        register real_t t_update_tt_stat;
        register real_t t_update_ff_stat;
        register real_t t_update_tf_stat;
        register real_t t_update_ttf_stat;
        register real_t t_update_dev_stat;

        pair < count_t, count_t > t_current_indices;

        t_current_indices.first = (count_t)( floor( (t_update_min / p_candidates.get_frequency_interval()) - p_candidates.get_frequency_index() ) );
        if( t_current_indices.first < 0 )
        {
            t_current_indices.first = 0;
        }

        t_current_indices.second = (count_t)( ceil( (t_update_max / p_candidates.get_frequency_interval()) - p_candidates.get_frequency_index() ) );
        if( t_current_indices.second > p_candidates.candidates().size() - 1 )
        {
            t_current_indices.second = p_candidates.candidates().size() - 1;
        }

        pair < count_t, count_t > t_next_indices;

        pointer< candidate > t_loop_point;
        list< pointer< candidate > > t_loop_points;
        register real_t t_loop_sum;
        register real_t t_loop_r_sum;
        register real_t t_loop_rt_sum;
        register real_t t_loop_rf_sum;
        register real_t t_loop_rtt_sum;
        register real_t t_loop_rff_sum;
        register real_t t_loop_rtf_sum;
        while( true )
        {
            msg_warning( coremsg, "  calculating properties:" << eom );
            msg_warning( coremsg, "      center is <" << t_update_center << ">" << eom );
            msg_warning( coremsg, "      min is <" << t_update_min << ">" << eom );
            msg_warning( coremsg, "      max is <" << t_update_max << ">" << eom );

            t_loop_point = NULL;
            t_loop_points.clear();
            t_loop_r_sum = p_line.r_sum();
            t_loop_rt_sum = p_line.rt_sum();
            t_loop_rf_sum = p_line.rf_sum();
            t_loop_rtt_sum = p_line.rtt_sum();
            t_loop_rff_sum = p_line.rff_sum();
            t_loop_rtf_sum = p_line.rtf_sum();
            for( t_index = t_current_indices.first; t_index <= t_current_indices.second; t_index++ )
            {
                t_loop_point = p_candidates.candidates().at( t_index );
                t_point_id = t_loop_point->id();
                t_point_frequency = t_loop_point->frequency();
                t_point_ratio = t_loop_point->ratio();

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

                msg_warning( coremsg, "    point:" << eom );
                msg_warning( coremsg, "      time is <" << t_point_time << ">" << eom );
                msg_warning( coremsg, "      frequency is <" << t_point_frequency << ">" << eom );
                msg_warning( coremsg, "      ratio is <" << t_point_ratio << ">" << eom );
                t_loop_points.push_back( t_loop_point );
                t_loop_sum += 1.;
                t_loop_r_sum += t_point_ratio;
                t_loop_rt_sum += t_point_ratio * t_point_duration;
                t_loop_rf_sum += t_point_ratio * t_point_frequency;
                t_loop_rtt_sum += t_point_ratio * t_point_duration * t_point_duration;
                t_loop_rff_sum += t_point_ratio * t_point_frequency * t_point_frequency;
                t_loop_rtf_sum += t_point_ratio * t_point_duration * t_point_frequency;
            }

            t_update_tt_stat = t_loop_r_sum * t_loop_rtt_sum - t_loop_rt_sum * t_loop_rt_sum;
            t_update_ff_stat = t_loop_r_sum * t_loop_rff_sum - t_loop_rf_sum * t_loop_rf_sum;
            t_update_tf_stat = t_loop_r_sum * t_loop_rtf_sum - t_loop_rt_sum * t_loop_rf_sum;
            t_update_ttf_stat = t_loop_rtt_sum * t_loop_rf_sum - t_loop_rt_sum * t_loop_rtf_sum;

            t_update_frequency = t_update_ttf_stat / t_update_tt_stat;
            t_update_slope = t_update_tf_stat / t_update_tt_stat;

            msg_warning( coremsg, "      frequency is <" << t_update_frequency << ">" << eom );
            msg_warning( coremsg, "      slope is <" << t_update_slope << ">" << eom );

            t_update_dev_stat = t_loop_rff_sum;
            t_update_dev_stat += t_update_slope * t_update_slope * t_loop_rtt_sum;
            t_update_dev_stat += t_update_frequency * t_update_frequency * t_loop_r_sum;
            t_update_dev_stat -= 2. * t_update_slope * t_loop_rtf_sum;
            t_update_dev_stat -= 2. * t_update_frequency * t_loop_rf_sum;
            t_update_dev_stat += 2. * t_update_slope * t_update_frequency * t_loop_rt_sum;

            t_update_center = t_update_frequency + t_update_slope * t_point_duration;
            t_update_min = t_update_center - p_line.window();
            t_update_max = t_update_center + p_line.window();

            t_next_indices.first = (count_t) (floor( (t_update_min / p_candidates.get_frequency_interval()) - p_candidates.get_frequency_index() ));
            if( t_next_indices.first < 0 )
            {
                t_next_indices.first = 0;
            }

            t_next_indices.second = (count_t) (ceil( (t_update_max / p_candidates.get_frequency_interval()) - p_candidates.get_frequency_index() ));
            if( t_next_indices.second > p_candidates.candidates().size() - 1 )
            {
                t_next_indices.second = p_candidates.candidates().size() - 1;
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

        p_line.duration() = t_point_duration;
        p_line.frequency() = t_update_frequency;
        p_line.slope() = t_update_slope;
        p_line.correlation() = t_update_correlation;
        p_line.deviation() = t_update_deviation;
        p_line.r_sum() = t_loop_r_sum;
        p_line.rt_sum() = t_loop_rt_sum;
        p_line.rf_sum() = t_loop_rf_sum;
        p_line.rtt_sum() = t_loop_rtt_sum;
        p_line.rff_sum() = t_loop_rff_sum;
        p_line.rtt_sum() = t_loop_rtt_sum;
        p_line.rtf_sum() = t_loop_rtf_sum;
        msg_warning( coremsg, "    duration is <" << f_duration << ">" << eom );
        msg_warning( coremsg, "    frequency is <" << f_frequency << ">" << eom );
        msg_warning( coremsg, "    slope is <" << f_slope << ">" << eom );
        msg_warning( coremsg, "    correlation is <" << f_correlation << ">" << eom );
        msg_warning( coremsg, "    deviation is <" << f_deviation << ">" << eom );

        msg_warning( coremsg, "  calculating values:" << eom );
        register real_t t_loop_w_sum = f_w_sum;
        register real_t t_loop_wr_sum = f_wr_sum;
        register real_t t_loop_os_sum = 0.;
        register real_t t_loop_us_sum = 0.;
        for( t_index = t_current_indices.first; t_index <= t_current_indices.second; t_index++ )
        {
            t_point_frequency = t_index * p_ratios.get_frequency_interval();
            if( t_point_frequency < t_update_min )
            {
                continue;
            }
            if( t_point_frequency > t_update_max )
            {
                continue;
            }

            t_point_score = p_scores.raw()[ t_index - p_scores.get_frequency_index() ];
            if( t_point_score > 0. )
            {
                continue;
            }

            t_point_ratio = p_ratios.raw()[ t_index - p_ratios.get_frequency_index() ];
            t_point_weight = weight( t_point_frequency - t_update_center );

            t_loop_w_sum += t_point_weight;
            t_loop_wr_sum += t_point_ratio * t_point_weight;

            if( t_point_ratio >= f_ratio )
            {
                t_loop_os_sum += p_context.f_factor_over * pow( (t_point_ratio - f_ratio), p_context.f_power_over ) * t_point_weight;
                p_scores.raw()[ t_index - p_scores.get_frequency_index() ] = (real_t)( f_id );
            }
            else
            {
                t_loop_us_sum += p_context.f_factor_under * pow( (f_ratio - t_point_ratio), p_context.f_power_under ) * t_point_weight;
            }

            f_points.push_back( new point( t_point_time, t_point_frequency, t_point_ratio ) );
        }

        f_w_sum = t_loop_w_sum;
        f_wr_sum = t_loop_wr_sum;
        f_score += t_loop_os_sum - t_loop_us_sum;
        msg_warning( coremsg, "    score is <" << f_score << ">" << eom );

        f_ratio = f_wr_sum / f_w_sum;
        if( f_ratio < p_context.f_ratio_low )
        {
            f_ratio = p_context.f_ratio_low;
        }
        if( f_ratio > p_context.f_ratio_high )
        {
            f_ratio = p_context.f_ratio_high;
        }
        msg_warning( coremsg, "    ratio is <" << f_ratio << ">" << eom );

        if( f_points.size() > 2 )
        {
            f_window = p_context.f_tolerance * f_deviation;
            if( f_window < p_context.f_window_low )
            {
                f_window = p_context.f_window_low;
            }
            if( f_window > p_context.f_window_high )
            {
                f_window = p_context.f_window_high;
            }
        }
        else
        {
            f_window = p_context.f_window_high;
        }
        msg_warning( coremsg, "    window is <" << f_window << ">" << eom );

        msg_warning( coremsg, "    frequency is <" << p_line.frequency() << ">" << eom );
        msg_warning( coremsg, "    slope is <" << p_line.slope() << ">" << eom );
        msg_warning( coremsg, "    deviation is <" << p_line.deviation() << ">" << eom );
        msg_warning( coremsg, "    correlation is <" << p_line.correlation() << ">" << eom );
        msg_warning( coremsg, "    ratio is <" << p_line.ratio() << ">" << eom );
        msg_warning( coremsg, "    window is <" << p_line.window() << ">" << eom );

        return;
    }

}
