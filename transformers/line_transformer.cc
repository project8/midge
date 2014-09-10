#include "line_transformer.hh"
#include "pool.hh"

#include <cmath>

namespace midge
{

    line_transformer::line_transformer() :
            f_threshold( 1. ),
            f_window( 30.e-3 ),
            f_tolerance( 1. ),
            f_count( 8 ),
            f_hit_factor( 2. ),
            f_hit_power( 1. ),
            f_miss_factor( 1. ),
            f_miss_power( 2. ),
            f_score_low( 0. ),
            f_score_high( 10. ),
            f_length( 10 )
    {
    }

    line_transformer::~line_transformer()
    {
    }

    void line_transformer::initialize()
    {
        out_buffer< 0 >().initialize( f_length );

        line::set_threshold( f_threshold );
        line::set_window( f_window );
        line::set_tolerance( f_tolerance );
        line::set_count( f_count );
        line::set_hit_factor( f_hit_factor );
        line::set_hit_power( f_hit_power );
        line::set_miss_factor( f_miss_factor );
        line::set_miss_power( f_miss_power );

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

        while( true )
        {
            t_command = in_stream< 0 >().get();
            t_input = in_stream< 0 >().data();
            t_output = out_stream< 0 >().data();

            if( t_command == stream::s_start )
            {
                t_output->points().resize( t_input->points().size() );
                t_output->set_size( t_input->points().size() );
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
                t_output->set_size( t_input->points().size() );
                t_output->set_time_interval( t_input->get_time_interval() );
                t_output->set_time_index( t_input->get_time_index() );
                t_output->set_frequency_interval( t_input->get_frequency_interval() );
                t_output->set_frequency_index( t_input->get_frequency_index() );

                line::set_data( t_input );

                line_it t_line_it;
                pointer< line > t_line;
                pointer< point > t_point;
                for( t_line_it = t_confirmed_lines.begin(); t_line_it != t_confirmed_lines.end(); t_line_it++ )
                {
                    t_line = *t_line_it;

                    //msg_warning( coremsg, "**   updating confirmed line <" << t_line->id() << "> **" << eom );
                    t_line->update();

                    if( t_line->score() < f_score_low )
                    {
                        //msg_warning( coremsg, "**   will complete confirmed line <" << t_line->id() << "> **" << eom );
                        t_line_complete_stack.push( t_line_it );
                    }
                }
                for( t_line_it = t_candidate_lines.begin(); t_line_it != t_candidate_lines.end(); t_line_it++ )
                {
                    t_line = *t_line_it;

                    //msg_warning( coremsg, "**   updating candidate line <" << t_line->id() << "> **" << eom );
                    t_line->update();

                    if( t_line->score() > f_score_high )
                    {
                        //msg_warning( coremsg, "**   will confirm candidate line <" << t_line->id() << "> **" << eom );
                        t_line_confirm_stack.push( t_line_it );
                    }
                    if( t_line->score() < f_score_low )
                    {
                        //msg_warning( coremsg, "**   will discard candidate line <" << t_line->id() << "> **" << eom );
                        t_line_discard_stack.push( t_line_it );
                    }
                }
                for( index_t t_index = 0; t_index < t_input->points().size(); t_index++ )
                {
                    t_point = t_input->points().at( t_index );
                    if( (t_point->id() == 0) && (t_point->ratio() > f_threshold) )
                    {
                        //pool< line >::allocate( t_line );
                        t_line = new line( t_index );

                        t_line_it = t_candidate_lines.insert( t_candidate_lines.end(), t_line );

                        //msg_warning( coremsg, "**   initializing created line <" << t_line->id() << "> **" << eom );
                        t_line->update();

                        if( t_line->score() < f_score_low )
                        {
                            //msg_warning( coremsg, "**   will discard created line <" << t_line->id() << "> **" << eom );
                            t_line_discard_stack.push( t_line_it );
                        }
                    }
                }

                //msg_warning( coremsg, "** completing lines **" << eom );
                while( t_line_complete_stack.empty() == false )
                {
                    t_line = *(t_line_complete_stack.top());
                    //msg_warning( coremsg, "**   completing line <" << t_line->id() << "> **" << eom );
                    t_complete_lines.push_back( t_line );

                    t_confirmed_lines.erase( t_line_complete_stack.top() );
                    t_line_complete_stack.pop();
                }

                //msg_warning( coremsg, "** confirming lines **" << eom );
                while( t_line_confirm_stack.empty() == false )
                {
                    t_line = *(t_line_confirm_stack.top());
                    //msg_warning( coremsg, "**   confirming line <" << t_line->id() << "> **" << eom );
                    t_confirmed_lines.push_back( t_line );

                    t_candidate_lines.erase( t_line_confirm_stack.top() );
                    t_line_confirm_stack.pop();
                }

                //msg_warning( coremsg, "** discarding lines **" << eom );
                while( t_line_discard_stack.empty() == false )
                {
                    t_line = *(t_line_discard_stack.top());
                    //msg_warning( coremsg, "**   discarding line <" << t_line->id() << "> **" << eom );
                    //pool< line >::free( t_line );
                    t_line = NULL;

                    t_candidate_lines.erase( t_line_discard_stack.top() );
                    t_line_discard_stack.pop();
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

}
