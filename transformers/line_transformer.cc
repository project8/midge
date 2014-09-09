#include "line_transformer.hh"
#include "pool.hh"

#include <cmath>

namespace midge
{

    line_transformer::line_transformer() :
            f_ratio_seed( 1. ),
            f_ratio_low( 0. ),
            f_ratio_high( 10. ),
            f_window_seed( 30.e-3 ),
            f_window_low( 10.e-3 ),
            f_window_high( 30.e-3 ),
            f_count( 8 ),
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

        enum_t t_command;

        point_data* t_input;
        point_data* t_output;

        count_t t_line_id = 0;
        while( true )
        {
            t_command = in_stream< 0 >().get();
            t_input = in_stream< 0 >().data();
            t_output = out_stream< 0 >().data();

            if( t_command == stream::s_start )
            {
                t_output->points().resize( t_input->points().size() );
                t_output->set_time_interval( t_input->get_time_interval() );
                t_output->set_time_index( t_input->get_time_index() );
                t_output->set_frequency_interval( t_input->get_frequency_interval() );
                t_output->set_frequency_index( t_input->get_frequency_index() );

                out_stream< 0 >().set( stream::s_start );
                continue;
            }

            if( t_command == stream::s_run )
            {
                t_output->points().resize( t_input->points().size() );
                t_output->set_time_interval( t_input->get_time_interval() );
                t_output->set_time_index( t_input->get_time_index() );
                t_output->set_frequency_interval( t_input->get_frequency_interval() );
                t_output->set_frequency_index( t_input->get_frequency_index() );

                line_it t_line_it;
                pointer< line > t_line;
                pointer< point > t_point;
                for( t_line_it = t_confirmed_lines.begin(); t_line_it != t_confirmed_lines.end(); t_line_it++ )
                {
                    t_line = *t_line_it;

                    msg_warning( coremsg, "**   updating confirmed line <" << t_line->id() << "> **" << eom );
                    update_line( *t_input, *t_line );

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
                    update_line( *t_input, *t_line );

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
                for( index_t t_index = 0; t_index < t_input->points().size(); t_index++ )
                {
                    t_point = t_input->points().at( t_index );
                    if( (t_point->id() == 0) && (t_point->ratio() > f_ratio_seed) )
                    {
                        //pool< line >::allocate( t_line );
                        t_line = new line();
                        t_line->id() = ++t_line_id;
                        t_line->time() = t_point->time();
                        t_line->frequency() = t_point->frequency();
                        t_line->ratio() = f_ratio_seed;
                        t_line->window() = f_window_seed;

                        t_line_it = t_candidate_lines.insert( t_candidate_lines.end(), t_line );

                        msg_warning( coremsg, "**   initializing created line <" << t_line->id() << "> **" << eom );
                        initialize_line( *t_input, *t_line );

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

                    //pool< line >::free( t_line );
                    t_line = NULL;
                }

                for( index_t t_index = 0; t_index < t_input->points().size(); t_index++ )
                {
                    t_output->points().at( t_index ) = new point();
                    t_output->points().at( t_index )->id() = t_input->points().at( t_index )->id();
                    t_output->points().at( t_index )->time() = t_input->points().at( t_index )->time();
                    t_output->points().at( t_index )->frequency() = t_input->points().at( t_index )->frequency();
                    t_output->points().at( t_index )->ratio() = t_input->points().at( t_index )->ratio();
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

    void line_transformer::initialize_line( point_data& p_points, line& p_line )
    {
        msg_warning( coremsg, "line <" << p_line.id() << "> initializing:" << eom );

        register index_t t_index;
        register count_t t_point_id;
        register real_t t_point_time = p_line.time();
        register real_t t_point_frequency;
        register real_t t_point_ratio;

        register real_t t_init_center = p_line.frequency();
        register real_t t_init_min = t_init_center - p_line.window();
        register real_t t_init_max = t_init_center + p_line.window();
        register real_t t_init_dev_stat;

        pair< index_t, index_t > t_current_indices;

        t_current_indices.first = (count_t) (floor( (t_init_min / p_points.get_frequency_interval()) - p_points.get_frequency_index() ));
        if( t_current_indices.first < 0 )
        {
            t_current_indices.first = 0;
        }

        t_current_indices.second = (count_t) (ceil( (t_init_max / p_points.get_frequency_interval()) - p_points.get_frequency_index() ));
        if( t_current_indices.second > p_points.points().size() - 1 )
        {
            t_current_indices.second = p_points.points().size() - 1;
        }

        pair< index_t, index_t > t_next_indices;

        pointer< point > t_loop_point;
        list< pointer< point > > t_loop_points;
        register count_t t_loop_count;
        register real_t t_loop_sum;
        register real_t t_loop_r_sum;
        register real_t t_loop_rf_sum;
        register real_t t_loop_rff_sum;
        while( true )
        {
            msg_warning( coremsg, "  calculating properties:" << eom );
            msg_warning( coremsg, "    center is <" << t_init_center << ">" << eom );
            msg_warning( coremsg, "    min is <" << t_init_min << ">" << eom );
            msg_warning( coremsg, "    max is <" << t_init_max << ">" << eom );

            t_loop_point = NULL;
            t_loop_points.clear();
            t_loop_count = 0;
            t_loop_sum = 0.;
            t_loop_r_sum = 0.;
            t_loop_rf_sum = 0.;
            t_loop_rff_sum = 0.;
            for( t_index = t_current_indices.first; t_index <= t_current_indices.second; t_index++ )
            {
                t_loop_point = p_points.points().at( t_index );
                t_point_id = t_loop_point->id();
                t_point_frequency = t_loop_point->frequency();
                t_point_ratio = t_loop_point->ratio();

                msg_warning( coremsg, "    point:" << eom );
                msg_warning( coremsg, "      time is <" << t_point_time << ">" << eom );
                msg_warning( coremsg, "      frequency is <" << t_point_frequency << ">" << eom );
                msg_warning( coremsg, "      ratio is <" << t_point_ratio << ">" << eom );

                if( t_point_id > 0 )
                {
                    continue;
                }
//                if( t_point_frequency < t_init_min )
//                {
//                    continue;
//                }
//                if( t_point_frequency > t_init_max )
//                {
//                    continue;
//                }

                msg_warning( coremsg, "      point accepted" << eom );
                t_loop_points.push_back( t_loop_point );
                t_loop_count += 1;
                t_loop_sum += t_point_ratio;

                if( t_point_ratio < p_line.ratio() )
                {
                    continue;
                }

                msg_warning( coremsg, "      point accumulated" << eom );
                t_loop_r_sum += t_point_ratio;
                t_loop_rf_sum += t_point_ratio * t_point_frequency;
                t_loop_rff_sum += t_point_ratio * t_point_frequency * t_point_frequency;
            }

            t_init_center = t_loop_rf_sum / t_loop_r_sum;
            t_init_min = t_init_center - p_line.window();
            t_init_max = t_init_center + p_line.window();

            t_next_indices.first = (count_t) (floor( (t_init_min / p_points.get_frequency_interval()) - p_points.get_frequency_index() ));
            if( t_next_indices.first < 0 )
            {
                t_next_indices.first = 0;
            }

            t_next_indices.second = (count_t) (ceil( (t_init_max / p_points.get_frequency_interval()) - p_points.get_frequency_index() ));
            if( t_next_indices.second > p_points.points().size() - 1 )
            {
                t_next_indices.second = p_points.points().size() - 1;
            }

            if( t_current_indices == t_next_indices )
            {
                for( t_index = t_current_indices.first; t_index <= t_current_indices.second; t_index++ )
                {
                    t_loop_point = p_points.points().at( t_index );
                    t_loop_point->id() = p_line.id();
                }
                break;
            }
            else
            {
                t_current_indices = t_next_indices;
            }
        }

        p_line.local().push_back( t_loop_points );
        p_line.local_count() = t_loop_count;
        p_line.local_sum() = t_loop_sum;
        p_line.local_r_sum() = t_loop_r_sum;
        p_line.local_rf_sum() = t_loop_rf_sum;
        p_line.local_rff_sum() = t_loop_rff_sum;

        p_line.frequency() = t_init_center;

        t_init_dev_stat = t_loop_rff_sum;
        t_init_dev_stat += t_init_center * t_init_center * t_loop_r_sum;
        t_init_dev_stat -= 2. * t_init_center * t_loop_rf_sum;
        if( t_init_dev_stat > 0. )
        {
            p_line.deviation() = sqrt( t_init_dev_stat / t_loop_r_sum );
        }
        else
        {
            p_line.deviation() = 0.;
        }

        if( p_line.local_count() > f_count )
        {
            p_line.ratio() = p_line.local_r_sum() / p_line.local_count();
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

        msg_warning( coremsg, "  time is <" << p_line.time() << ">" << eom );
        msg_warning( coremsg, "  frequency is <" << p_line.frequency() << ">" << eom );
        msg_warning( coremsg, "  ratio is <" << p_line.ratio() << ">" << eom );
        msg_warning( coremsg, "  window is <" << p_line.window() << ">" << eom );
        msg_warning( coremsg, "  slope is <" << p_line.slope() << ">" << eom );
        msg_warning( coremsg, "  duration is <" << p_line.duration() << ">" << eom );
        msg_warning( coremsg, "  deviation is <" << p_line.deviation() << ">" << eom );
        msg_warning( coremsg, "  correlation is <" << p_line.correlation() << ">" << eom );

        return;
    }

    void line_transformer::update_line( point_data& p_points, line& p_line )
    {
        msg_warning( coremsg, "line <" << p_line.id() << "> updating:" << eom );

        register index_t t_index;
        list< pointer< point > >::iterator t_point_it;
        list< list< pointer< point > > >::iterator t_group_it;

        register count_t t_point_id;
        register real_t t_point_time = p_points.get_time_interval() * p_points.get_time_index();
        register real_t t_point_duration = t_point_time - p_line.time();
        register real_t t_point_frequency;
        register real_t t_point_ratio;

        register real_t t_update_frequency = p_line.frequency();
        register real_t t_update_slope = p_line.slope();
        register real_t t_update_deviation = p_line.deviation();
        register real_t t_update_correlation = p_line.correlation();
        register real_t t_update_center = t_update_frequency + t_update_slope * t_point_duration;
        register real_t t_update_min = t_update_center - p_line.window();
        register real_t t_update_max = t_update_center + p_line.window();

        register real_t t_update_tt_stat;
        register real_t t_update_ff_stat;
        register real_t t_update_tf_stat;
        register real_t t_update_ttf_stat;
        register real_t t_update_dev_stat;

        pair< index_t, index_t > t_current_indices;

        t_current_indices.first = (count_t) (floor( (t_update_min / p_points.get_frequency_interval()) - p_points.get_frequency_index() ));
        if( t_current_indices.first < 0 )
        {
            t_current_indices.first = 0;
        }

        t_current_indices.second = (count_t) (ceil( (t_update_max / p_points.get_frequency_interval()) - p_points.get_frequency_index() ));
        if( t_current_indices.second > p_points.points().size() - 1 )
        {
            t_current_indices.second = p_points.points().size() - 1;
        }

        pair< index_t, index_t > t_next_indices;

        pointer< point > t_loop_point;
        list< pointer< point > > t_loop_points;
        register count_t t_loop_local_count;
        register real_t t_loop_local_sum;
        register real_t t_loop_local_r_sum;
        register real_t t_loop_local_rt_sum;
        register real_t t_loop_local_rf_sum;
        register real_t t_loop_local_rtt_sum;
        register real_t t_loop_local_rff_sum;
        register real_t t_loop_local_rtf_sum;
        register count_t t_loop_total_count;
        register real_t t_loop_total_sum;
        register real_t t_loop_total_r_sum;
        register real_t t_loop_total_rt_sum;
        register real_t t_loop_total_rf_sum;
        register real_t t_loop_total_rtt_sum;
        register real_t t_loop_total_rff_sum;
        register real_t t_loop_total_rtf_sum;
        while( true )
        {
            msg_warning( coremsg, "  calculating properties:" << eom );
            msg_warning( coremsg, "    center is <" << t_update_center << ">" << eom );
            msg_warning( coremsg, "    min is <" << t_update_min << ">" << eom );
            msg_warning( coremsg, "    max is <" << t_update_max << ">" << eom );
            msg_warning( coremsg, "    first is <" << t_current_indices.first << ">" << eom );
            msg_warning( coremsg, "    last is <" << t_current_indices.second << ">" << eom );

            t_loop_point = NULL;
            t_loop_points.clear();
            t_loop_local_count = p_line.local_count();
            t_loop_local_sum = p_line.local_sum();
            t_loop_local_r_sum = p_line.local_r_sum();
            t_loop_local_rt_sum = p_line.local_rt_sum();
            t_loop_local_rf_sum = p_line.local_rf_sum();
            t_loop_local_rtt_sum = p_line.local_rtt_sum();
            t_loop_local_rff_sum = p_line.local_rff_sum();
            t_loop_local_rtf_sum = p_line.local_rtf_sum();
            t_loop_total_count = p_line.global_count();
            t_loop_total_sum = p_line.global_sum();
            t_loop_total_r_sum = p_line.global_r_sum();
            t_loop_total_rt_sum = p_line.global_rt_sum();
            t_loop_total_rf_sum = p_line.global_rf_sum();
            t_loop_total_rtt_sum = p_line.global_rtt_sum();
            t_loop_total_rff_sum = p_line.global_rff_sum();
            t_loop_total_rtf_sum = p_line.global_rtf_sum();
            for( t_index = t_current_indices.first; t_index <= t_current_indices.second; t_index++ )
            {
                t_loop_point = p_points.points().at( t_index );
                t_point_id = t_loop_point->id();
                t_point_frequency = t_loop_point->frequency();
                t_point_ratio = t_loop_point->ratio();

                msg_warning( coremsg, "    point:" << eom );
                msg_warning( coremsg, "      time is <" << t_point_time << ">" << eom );
                msg_warning( coremsg, "      frequency is <" << t_point_frequency << ">" << eom );
                msg_warning( coremsg, "      ratio is <" << t_point_ratio << ">" << eom );

                if( t_point_id > 0 )
                {
                    continue;
                }
//                if( t_point_frequency < t_update_min )
//                {
//                    continue;
//                }
//                if( t_point_frequency > t_update_max )
//                {
//                    continue;
//                }

                msg_warning( coremsg, "      point accepted" << eom );
                t_loop_points.push_back( t_loop_point );
                t_loop_local_count += 1;
                t_loop_local_sum += t_point_ratio;

                if( t_point_ratio < p_line.ratio() )
                {
                    continue;
                }

                msg_warning( coremsg, "      point accumulated" << eom );
                t_loop_local_r_sum += t_point_ratio;
                t_loop_local_rt_sum += t_point_ratio * t_point_duration;
                t_loop_local_rf_sum += t_point_ratio * t_point_frequency;
                t_loop_local_rtt_sum += t_point_ratio * t_point_duration * t_point_duration;
                t_loop_local_rff_sum += t_point_ratio * t_point_frequency * t_point_frequency;
                t_loop_local_rtf_sum += t_point_ratio * t_point_duration * t_point_frequency;
            }
            t_loop_total_count += t_loop_local_count;
            t_loop_total_sum += t_loop_local_sum;
            t_loop_total_r_sum += t_loop_local_r_sum;
            t_loop_total_rt_sum += t_loop_local_rt_sum;
            t_loop_total_rf_sum += t_loop_local_rf_sum;
            t_loop_total_rtt_sum += t_loop_local_rtt_sum;
            t_loop_total_rff_sum += t_loop_local_rff_sum;
            t_loop_total_rtf_sum += t_loop_local_rtf_sum;

            msg_warning( coremsg, "    total r sum is <" << t_loop_total_r_sum << ">" << eom );
            msg_warning( coremsg, "    total rt sum is <" << t_loop_total_rt_sum << ">" << eom );
            msg_warning( coremsg, "    total rf sum is <" << t_loop_total_rf_sum << ">" << eom );
            msg_warning( coremsg, "    total rtt sum is <" << t_loop_total_rtt_sum << ">" << eom );
            msg_warning( coremsg, "    total rff sum is <" << t_loop_total_rff_sum << ">" << eom );
            msg_warning( coremsg, "    total rtf sum is <" << t_loop_total_rtf_sum << ">" << eom );

            t_update_tt_stat = t_loop_total_r_sum * t_loop_total_rtt_sum - t_loop_total_rt_sum * t_loop_total_rt_sum;
            t_update_ff_stat = t_loop_total_r_sum * t_loop_total_rff_sum - t_loop_total_rf_sum * t_loop_total_rf_sum;
            t_update_tf_stat = t_loop_total_r_sum * t_loop_total_rtf_sum - t_loop_total_rt_sum * t_loop_total_rf_sum;
            t_update_ttf_stat = t_loop_total_rtt_sum * t_loop_total_rf_sum - t_loop_total_rt_sum * t_loop_total_rtf_sum;

            if( t_update_tt_stat != 0. )
            {
                if( t_update_ff_stat != 0. )
                {
                    t_update_correlation = sqrt( (t_update_tf_stat * t_update_tf_stat) / (t_update_tt_stat * t_update_ff_stat) );
                }
                else
                {
                    t_update_correlation = 0.;
                }
                t_update_frequency = t_update_ttf_stat / t_update_tt_stat;
                t_update_slope = t_update_tf_stat / t_update_tt_stat;
            }
            else
            {
                t_update_frequency = t_loop_total_rf_sum / t_loop_total_r_sum;
                t_update_slope = 0.;
                t_update_correlation = 0.;
            }

            msg_warning( coremsg, "    frequency is <" << t_update_frequency << ">" << eom );
            msg_warning( coremsg, "    slope is <" << t_update_slope << ">" << eom );
            msg_warning( coremsg, "    correlation is <" << t_update_correlation << ">" << eom );

            t_update_dev_stat = t_loop_total_rff_sum;
            t_update_dev_stat += t_update_slope * t_update_slope * t_loop_total_rtt_sum;
            t_update_dev_stat += t_update_frequency * t_update_frequency * t_loop_total_r_sum;
            t_update_dev_stat -= 2. * t_update_slope * t_loop_total_rtf_sum;
            t_update_dev_stat -= 2. * t_update_frequency * t_loop_total_rf_sum;
            t_update_dev_stat += 2. * t_update_slope * t_update_frequency * t_loop_total_rt_sum;

            if( t_update_dev_stat > 0. )
            {
                t_update_deviation = sqrt( t_update_dev_stat / t_loop_total_r_sum );
            }
            else
            {
                t_update_deviation = 0.;
            }

            t_update_center = t_update_frequency + t_update_slope * t_point_duration;
            t_update_min = t_update_center - p_line.window();
            t_update_max = t_update_center + p_line.window();

            t_next_indices.first = (count_t) (floor( (t_update_min / p_points.get_frequency_interval()) - p_points.get_frequency_index() ));
            if( t_next_indices.first < 0 )
            {
                t_next_indices.first = 0;
            }

            t_next_indices.second = (count_t) (ceil( (t_update_max / p_points.get_frequency_interval()) - p_points.get_frequency_index() ));
            if( t_next_indices.second > p_points.points().size() - 1 )
            {
                t_next_indices.second = p_points.points().size() - 1;
            }

            if( t_current_indices == t_next_indices )
            {
                for( t_point_it = t_loop_points.begin(); t_point_it != t_loop_points.end(); t_point_it++ )
                {
                    (*t_point_it)->id() = p_line.id();
                }
                p_line.local().push_back( t_loop_points );
                break;
            }
            else
            {
                t_current_indices = t_next_indices;
            }
        }

        p_line.frequency() = t_update_frequency;
        p_line.slope() = t_update_slope;
        p_line.duration() = t_point_duration;
        p_line.correlation() = t_update_correlation;
        p_line.deviation() = t_update_deviation;
        p_line.local_count() = t_loop_local_count;
        p_line.local_sum() = t_loop_local_sum;
        p_line.local_r_sum() = t_loop_local_r_sum;
        p_line.local_rt_sum() = t_loop_local_rt_sum;
        p_line.local_rf_sum() = t_loop_local_rf_sum;
        p_line.local_rtt_sum() = t_loop_local_rtt_sum;
        p_line.local_rff_sum() = t_loop_local_rff_sum;
        p_line.local_rtt_sum() = t_loop_local_rtt_sum;
        p_line.local_rtf_sum() = t_loop_local_rtf_sum;

        if( p_line.local_count() < f_count )
        {
            msg_warning( coremsg, "  nascent stage:" << eom );
            p_line.ratio() = f_ratio_seed;
            p_line.window() = f_window_seed;
        }
        else
        {
            msg_warning( coremsg, "  mature stage:" << eom );
            p_line.ratio() = p_line.local_sum() / p_line.local_count();
            p_line.window() = f_tolerance * p_line.deviation();
            if( p_line.window() < f_window_low )
            {
                p_line.window() = f_window_low;
            }
            if( p_line.window() > f_window_high )
            {
                p_line.window() = f_window_high;
            }
            if( p_line.local_count() - p_line.local().front().size() >= f_count )
            {
                while( p_line.local().front().empty() == false )
                {
                    t_loop_point = p_line.local().front().front();
                    t_point_time = t_loop_point->time();
                    t_point_frequency = t_loop_point->frequency();
                    t_point_ratio = t_loop_point->ratio();

                    msg_warning( coremsg, "    point:" << eom );
                    msg_warning( coremsg, "      time is <" << t_point_time << ">" << eom );
                    msg_warning( coremsg, "      frequency is <" << t_point_frequency << ">" << eom );
                    msg_warning( coremsg, "      ratio is <" << t_point_ratio << ">" << eom );

                    p_line.local_count() -= 1;
                    p_line.local_sum() -= t_point_ratio;
                    p_line.local_r_sum() -= t_point_ratio;
                    p_line.local_rt_sum() -= t_point_ratio * t_point_time;
                    p_line.local_rf_sum() -= t_point_ratio * t_point_frequency;
                    p_line.local_rtt_sum() -= t_point_ratio * t_point_time * t_point_time;
                    p_line.local_rff_sum() -= t_point_ratio * t_point_frequency * t_point_frequency;
                    p_line.local_rtf_sum() -= t_point_ratio * t_point_time * t_point_frequency;

                    if( t_point_ratio >= p_line.ratio() )
                    {
                        msg_warning( coremsg, "      point retained" << eom );
                        p_line.global().push_back( t_loop_point );
                        p_line.global_count() += 1;
                        p_line.global_sum() += t_point_ratio;
                        p_line.global_r_sum() += t_point_ratio;
                        p_line.global_rt_sum() += t_point_ratio * t_point_time;
                        p_line.global_rf_sum() += t_point_ratio * t_point_frequency;
                        p_line.global_rtt_sum() += t_point_ratio * t_point_time * t_point_time;
                        p_line.global_rff_sum() += t_point_ratio * t_point_frequency * t_point_frequency;
                        p_line.global_rtf_sum() += t_point_ratio * t_point_time * t_point_frequency;
                    }
                    else
                    {
                        msg_warning( coremsg, "      point freed" << eom );
                        pool< point >::free( t_loop_point );
                    }

                    p_line.local().front().pop_front();
                }
                p_line.local().pop_front();
            }
        }

        msg_warning( coremsg, "    time is <" << p_line.time() << ">" << eom );
        msg_warning( coremsg, "    frequency is <" << p_line.frequency() << ">" << eom );
        msg_warning( coremsg, "    ratio is <" << p_line.ratio() << ">" << eom );
        msg_warning( coremsg, "    window is <" << p_line.window() << ">" << eom );
        msg_warning( coremsg, "    slope is <" << p_line.slope() << ">" << eom );
        msg_warning( coremsg, "    duration is <" << p_line.duration() << ">" << eom );
        msg_warning( coremsg, "    deviation is <" << p_line.deviation() << ">" << eom );
        msg_warning( coremsg, "    correlation is <" << p_line.correlation() << ">" << eom );

        return;
    }

}
