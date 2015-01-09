#include "event_transformer.hh"

#include <cmath>

namespace midge
{

    event_transformer::event_transformer() :
                f_frequency( 10.0e6 ),
                f_time( 500.0e-6 ),
                f_tolerance( 1. ),
                f_delay( 2.0e-3 ),
                f_length( 10 )
    {
    }

    event_transformer::~event_transformer()
    {
    }

    void event_transformer::initialize()
    {
        out_buffer< 0 >().initialize( f_length );

        event::set_frequency( f_frequency );
        event::set_time( f_time );
        event::set_tolerance( f_tolerance );
        event::set_delay( f_delay );

        return;
    }

    void event_transformer::execute()
    {
        event_list t_active_events;
        event_stack t_event_complete_stack;

        enum_t t_command;
        line_data* t_lines;
        event_data* t_events;

        count_t t_size;
        real_t t_time_interval;
        count_t t_time_index;
        real_t t_frequency_interval;
        count_t t_frequency_index;

        event_it t_event_it;
        event* t_event;

        while( true )
        {
            t_command = in_stream< 0 >().get();
            t_lines = in_stream< 0 >().data();
            t_events = out_stream< 0 >().data();

            if( t_command == stream::s_start )
            {
                t_size = t_lines->size();
                t_frequency_interval = t_lines->frequency_interval();
                t_frequency_index = t_lines->frequency_index();
                t_time_interval = t_lines->time_interval();
                t_time_index = t_lines->time_index();

                t_events->size() = t_size;
                t_events->frequency_interval() = t_frequency_interval;
                t_events->frequency_index() = t_frequency_index;
                t_events->time_interval() = t_time_interval;
                t_events->time_index() = t_time_index;

                t_events->events().clear();

                out_stream< 0 >().set( stream::s_start );
                continue;
            }

            if( t_command == stream::s_run )
            {
                t_size = t_lines->size();
                t_frequency_interval = t_lines->frequency_interval();
                t_frequency_index = t_lines->frequency_index();
                t_time_interval = t_lines->time_interval();
                t_time_index = t_lines->time_index();

                t_events->size() = t_size;
                t_events->frequency_interval() = t_frequency_interval;
                t_events->frequency_index() = t_frequency_index;
                t_events->time_interval() = t_time_interval;
                t_events->time_index() = t_time_index;

                t_events->events().clear();
                event::set_line_data( t_lines );

                for( t_event_it = t_active_events.begin(); t_event_it != t_active_events.end(); t_event_it++ )
                {
                    t_event = *t_event_it;

                    if( t_event->update() == false )
                    {
                        t_event_complete_stack.push( t_event_it );
                    }
                }

                for( index_t t_index = 0; t_index < t_lines->lines().size(); t_index++ )
                {
                    if( t_lines->lines().at( t_index ).free() == true )
                    {
                        t_event = new event();
                        t_event->initialize( t_index );

                        t_active_events.push_back( t_event );
                    }
                }

                while( t_event_complete_stack.empty() == false )
                {
                    t_event = *(t_event_complete_stack.top());

                    t_events->events().push_back( *t_event );
                    delete t_event;

                    t_active_events.erase( t_event_complete_stack.top() );
                    t_event_complete_stack.pop();
                }

                out_stream< 0 >().set( stream::s_run );
                continue;
            }

            if( t_command == stream::s_stop )
            {
                t_events->size() = t_size;
                t_events->time_interval() = t_time_interval;
                t_events->time_index() = t_time_index;
                t_events->frequency_interval() = t_frequency_interval;
                t_events->frequency_index() = t_frequency_index;

                t_events->events().clear();
                event::set_line_data( t_lines );

                for( t_event_it = t_active_events.begin(); t_event_it != t_active_events.end(); t_event_it++ )
                {
                    t_event = *t_event_it;

                    if( t_event->update() == false )
                    {
                        t_event_complete_stack.push( t_event_it );
                    }
                }

                for( index_t t_index = 0; t_index < t_lines->lines().size(); t_index++ )
                {
                    if( t_lines->lines().at( t_index ).free() == true )
                    {
                        t_event = new event();
                        t_event->initialize( t_index );

                        t_active_events.push_back( t_event );
                    }
                }

                while( t_event_complete_stack.empty() == false )
                {
                    t_event = *(t_event_complete_stack.top());

                    t_events->events().push_back( *t_event );
                    delete t_event;

                    t_active_events.erase( t_event_complete_stack.top() );
                    t_event_complete_stack.pop();
                }

                for( t_event_it = t_active_events.begin(); t_event_it != t_active_events.end(); t_event_it++ )
                {
                    t_event = *t_event_it;

                    t_events->events().push_back( *t_event );
                    delete t_event;
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

    void event_transformer::finalize()
    {
        out_buffer< 0 >().finalize();
        return;
    }

}
