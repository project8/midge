#include "line_transformer.hh"

#include <cmath>

namespace midge
{

    line_transformer::line_transformer() :
                f_threshold( 3.0 ),
                f_window( 65.e-3 ),
                f_width( 35.e-3 ),
                f_trim( 0.5 ),
                f_block( 5.0 ),
                f_count( 20 ),
                f_score( 0. ),
                f_quality( 10. ),
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
        line::set_trim( f_trim );
        line::set_block( f_block );
        line::set_count( f_count );

        return;
    }

    void line_transformer::execute()
    {
        line_list t_active_lines;
        line_stack t_line_complete_stack;
        line_stack t_line_discard_stack;

        enum_t t_command;
        point_data* t_points;
        line_data* t_lines;

        count_t t_size;
        real_t t_time_interval;
        count_t t_time_index;
        real_t t_frequency_interval;
        count_t t_frequency_index;

        line_it t_line_it;
        line* t_line;

        while( true )
        {
            t_command = in_stream< 0 >().get();
            t_points = in_stream< 0 >().data();
            t_lines = out_stream< 0 >().data();

            if( t_command == stream::s_start )
            {
                t_size = t_points->size();
                t_frequency_interval = t_points->frequency_interval();
                t_frequency_index = t_points->frequency_index();
                t_time_interval = t_points->time_interval();
                t_time_index = t_points->time_index();

                t_lines->size() = t_size;
                t_lines->frequency_interval() = t_frequency_interval;
                t_lines->frequency_index() = t_frequency_index;
                t_lines->time_interval() = t_time_interval;
                t_lines->time_index() = t_time_index;

                t_lines->lines().clear();

                out_stream< 0 >().set( stream::s_start );
                continue;
            }

            if( t_command == stream::s_run )
            {
                t_size = t_points->size();
                t_frequency_interval = t_points->frequency_interval();
                t_frequency_index = t_points->frequency_index();
                t_time_interval = t_points->time_interval();
                t_time_index = t_points->time_index();

                t_lines->size() = t_size;
                t_lines->frequency_interval() = t_frequency_interval;
                t_lines->frequency_index() = t_frequency_index;
                t_lines->time_interval() = t_time_interval;
                t_lines->time_index() = t_time_index;

                t_lines->lines().clear();
                line::set_point_data( t_points );

                //msg_warning( coremsg, "** updating active lines **" << eom );
                for( t_line_it = t_active_lines.begin(); t_line_it != t_active_lines.end(); t_line_it++ )
                {
                    t_line = *t_line_it;

                    //msg_warning( coremsg, "**   updating active line <" << t_line->id() << "> **" << eom );
                    if( t_line->update() == true )
                    {
                        if( t_line->score() < f_score )
                        {
                            t_line->finalize();

                            if( t_line->quality() > f_quality )
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
                    else
                    {
                        //msg_warning( coremsg, "**   will discard active line <" << t_line->id() << "> **" << eom );
                        t_line_discard_stack.push( t_line_it );
                    }
                }

                //msg_warning( coremsg, "** creating new lines **" << eom );
                for( index_t t_index = 0; t_index < t_points->points().size(); t_index++ )
                {
                    if( t_points->points().at( t_index ).ratio() > f_threshold )
                    {
                        t_line = new line();

                        //msg_warning( coremsg, "**   initializing created line <" << t_line->id() << "> **" << eom );
                        t_line->initialize( t_index );

                        //msg_warning( coremsg, "**   updating created line <" << t_line->id() << "> **" << eom );
                        if( t_line->update() == true )
                        {
                            //msg_warning( coremsg, "**   activating created line <" << t_line->id() << "> **" << eom );
                            t_active_lines.push_back( t_line );
                        }
                        else
                        {
                            //msg_warning( coremsg, "**   deleting created line <" << t_line->id() << "> **" << eom );
                            delete t_line;
                        }
                    }
                }

                //msg_warning( coremsg, "** completing lines **" << eom );
                while( t_line_complete_stack.empty() == false )
                {
                    t_line = *(t_line_complete_stack.top());

                    //msg_warning( coremsg, "**   completing line <" << t_line->id() << "> **" << eom );

                    t_lines->lines().push_back( *t_line );
                    delete t_line;

                    t_active_lines.erase( t_line_complete_stack.top() );
                    t_line_complete_stack.pop();
                }

                //msg_warning( coremsg, "** discarding lines **" << eom );
                while( t_line_discard_stack.empty() == false )
                {
                    t_line = *(t_line_discard_stack.top());

                    //msg_warning( coremsg, "**   discarding line <" << t_line->id() << "> **" << eom );

                    delete t_line;

                    t_active_lines.erase( t_line_discard_stack.top() );
                    t_line_discard_stack.pop();
                }

                out_stream< 0 >().set( stream::s_run );
                continue;
            }

            if( t_command == stream::s_stop )
            {
                t_size = t_points->size();
                t_frequency_interval = t_points->frequency_interval();
                t_frequency_index = t_points->frequency_index();
                t_time_interval = t_points->time_interval();
                t_time_index = t_points->time_index();

                t_lines->size() = t_size;
                t_lines->frequency_interval() = t_frequency_interval;
                t_lines->frequency_index() = t_frequency_index;
                t_lines->time_interval() = t_time_interval;
                t_lines->time_index() = t_time_index;

                t_lines->lines().clear();
                line::set_point_data( t_points );

                //msg_warning( coremsg, "** finalizing active lines **" << eom );
                for( t_line_it = t_active_lines.begin(); t_line_it != t_active_lines.end(); t_line_it++ )
                {
                    t_line = *t_line_it;

                    //msg_warning( coremsg, "**   finalizing active line <" << t_line->id() << "> **" << eom );
                    t_line->finalize();

                    if( t_line->quality() > f_quality )
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

                //msg_warning( coremsg, "** completing lines **" << eom );
                while( t_line_complete_stack.empty() == false )
                {
                    t_line = *(t_line_complete_stack.top());

                    //msg_warning( coremsg, "**   completing line <" << t_line->id() << "> **" << eom );

                    t_lines->lines().push_back( *t_line );
                    delete t_line;

                    t_active_lines.erase( t_line_complete_stack.top() );
                    t_line_complete_stack.pop();
                }

                //msg_warning( coremsg, "** discarding lines **" << eom );
                while( t_line_discard_stack.empty() == false )
                {
                    t_line = *(t_line_discard_stack.top());

                    //msg_warning( coremsg, "**   discarding line <" << t_line->id() << "> **" << eom );

                    delete t_line;

                    t_active_lines.erase( t_line_discard_stack.top() );
                    t_line_discard_stack.pop();
                }

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
