#include "line_transformer.hh"

#include <cmath>

namespace midge
{

    line_transformer::line_transformer() :
            f_threshold( 2.0 ),
            f_window( 65.e-3 ),
            f_width( 45.e-3 ),
            f_count( 20 ),
            f_score_low( 0. ),
            f_quality_high( 10. ),
            f_object_pool( 10000 ),
            f_pointer_pool( 100000 ),
            f_length( 10 )
    {
    }

    line_transformer::~line_transformer()
    {
    }

    void line_transformer::initialize()
    {
        out_buffer< 0 >().initialize( f_length );

        line::set_window( f_window );
        line::set_width( f_width );
        line::set_count( f_count );

        return;
    }

    void line_transformer::execute()
    {
        line_list t_active_lines;

        line_stack t_line_complete_stack;
        line_stack t_line_discard_stack;

        enum_t t_command;

        point_data* t_input;
        line_data* t_current;
        line_data* t_previous;

        line_it t_line_it;
        line* t_line;

        count_t t_id = 0;

        while( true )
        {
            t_command = in_stream< 0 >().get();
            t_input = in_stream< 0 >().data();
            t_current = out_stream< 0 >().data();

            if( t_command == stream::s_start )
            {
                t_current->lines().clear();
                t_current->set_size( t_input->get_size() );
                t_current->set_time_interval( t_input->get_time_interval() );
                t_current->set_time_index( t_input->get_time_index() );
                t_current->set_frequency_interval( t_input->get_frequency_interval() );
                t_current->set_frequency_index( t_input->get_frequency_index() );

                out_stream< 0 >().set( stream::s_start );
                continue;
            }

            if( t_command == stream::s_run )
            {
                t_current->lines().clear();
                t_current->set_size( t_input->get_size() );
                t_current->set_time_interval( t_input->get_time_interval() );
                t_current->set_time_index( t_input->get_time_index() );
                t_current->set_frequency_interval( t_input->get_frequency_interval() );
                t_current->set_frequency_index( t_input->get_frequency_index() );

                line::set_data( t_input );

                //msg_warning( coremsg, "** updating active lines <" << t_count << "> **" << eom );
                for( t_line_it = t_active_lines.begin(); t_line_it != t_active_lines.end(); t_line_it++ )
                {
                    t_line = *t_line_it;

                    //msg_warning( coremsg, "**   updating active line <" << t_line->id() << "> **" << eom );
                    t_line->update();

                    if( t_line->score() < f_score_low )
                    {
                        t_line->finalize();

                        if( t_line->quality() > f_quality_high )
                        {
                            //msg_warning( coremsg, "**   will complete active line <" << t_line->id() << "> **" << eom );
                            t_line_complete_stack.push( t_line_it );
                        }
                        else
                        {
                            //msg_warning( coremsg, "**   will discard active line <" << t_line->id() << "> **" << eom );
                            t_line_discard_stack.push( t_line_it );
                        }
                    }
                }

                //msg_warning( coremsg, "** creating new lines <" << t_count << "> **" << eom );
                for( index_t t_index = 0; t_index < t_input->points().size(); t_index++ )
                {
                    if( (t_input->points().at( t_index ).id() == 0) && (t_input->points().at( t_index ).ratio() > f_threshold) )
                    {
                        t_line = new line();
                        t_line->id() = ++t_id;

                        //msg_warning( coremsg, "**   initializing created line <" << t_line->id() << "> **" << eom );
                        t_line->initialize( t_index );

                        //msg_warning( coremsg, "**   updating created line <" << t_line->id() << "> **" << eom );
                        t_line->update();

                        //msg_warning( coremsg, "**   activating created line <" << t_line->id() << "> **" << eom );
                        t_active_lines.push_back( t_line );
                    }
                }

                //msg_warning( coremsg, "** completing lines <" << t_count << "> **" << eom );
                while( t_line_complete_stack.empty() == false )
                {
                    t_line = *(t_line_complete_stack.top());

                    //msg_warning( coremsg, "**   completing line <" << t_line->id() << "> **" << eom );

                    t_current->lines().push_back( *t_line );
                    delete t_line;

                    t_active_lines.erase( t_line_complete_stack.top() );
                    t_line_complete_stack.pop();
                }

                //msg_warning( coremsg, "** discarding lines <" << t_count << "> **" << eom );
                while( t_line_discard_stack.empty() == false )
                {
                    t_line = *(t_line_discard_stack.top());

                    //msg_warning( coremsg, "**   discarding line <" << t_line->id() << "> **" << eom );

                    delete t_line;

                    t_active_lines.erase( t_line_discard_stack.top() );
                    t_line_discard_stack.pop();
                }

                t_previous = t_current;

                out_stream< 0 >().set( stream::s_run );
                continue;
            }

            if( t_command == stream::s_stop )
            {
                t_current->lines().clear();
                t_current->set_size( t_previous->get_size() );
                t_current->set_time_interval( t_previous->get_time_interval() );
                t_current->set_time_index( t_previous->get_time_index() );
                t_current->set_frequency_interval( t_previous->get_frequency_interval() );
                t_current->set_frequency_index( t_previous->get_frequency_index() );

                //msg_warning( coremsg, "** finalizing active lines <" << t_count << "> **" << eom );
                for( t_line_it = t_active_lines.begin(); t_line_it != t_active_lines.end(); t_line_it++ )
                {
                    t_line = *t_line_it;

                    //msg_warning( coremsg, "**   finalizing active line <" << t_line->id() << "> **" << eom );
                    t_line->finalize();

                    if( t_line->quality() > f_quality_high )
                    {
                        //msg_warning( coremsg, "**   will complete active line <" << t_line->id() << "> **" << eom );
                        t_line_complete_stack.push( t_line_it );
                    }
                    else
                    {
                        //msg_warning( coremsg, "**   will discard active line <" << t_line->id() << "> **" << eom );
                        t_line_discard_stack.push( t_line_it );
                    }
                }

                //msg_warning( coremsg, "** completing lines <" << t_count << "> **" << eom );
                while( t_line_complete_stack.empty() == false )
                {
                    t_line = *(t_line_complete_stack.top());

                    //msg_warning( coremsg, "**   completing line <" << t_line->id() << "> **" << eom );

                    t_current->lines().push_back( *t_line );
                    delete t_line;

                    t_active_lines.erase( t_line_complete_stack.top() );
                    t_line_complete_stack.pop();
                }

                //msg_warning( coremsg, "** completing lines <" << t_count << "> **" << eom );
                while( t_line_discard_stack.empty() == false )
                {
                    t_line = *(t_line_discard_stack.top());

                    //msg_warning( coremsg, "**   discarding line <" << t_line->id() << "> **" << eom );

                    delete t_line;

                    t_active_lines.erase( t_line_discard_stack.top() );
                    t_line_discard_stack.pop();
                }

                out_stream< 0 >().set( stream::s_run );
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
