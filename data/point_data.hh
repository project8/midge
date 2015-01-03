#ifndef _midge_point_data_hh_
#define _midge_point_data_hh_

#include "point.hh"

#include <vector>
using std::vector;

namespace midge
{

    class point_data
    {
        public:
            point_data();
            virtual ~point_data();

        public:
            point& at( const count_t& p_index );
            const point& at( const count_t& p_index ) const;

            void set_size( const count_t& p_size );
            const count_t& get_size() const;

            void set_time_interval( const real_t& p_time_interval );
            const real_t& get_time_interval() const;

            void set_time_index( const count_t& p_time_index );
            const count_t& get_time_index() const;

            void set_frequency_interval( const real_t& p_frequency_interval );
            const real_t& get_frequency_interval() const;

            void set_frequency_index( const count_t& p_frequency_index );
            const count_t& get_frequency_index() const;

        protected:
            vector< point > f_data;
            count_t f_size;
            real_t f_time_interval;
            count_t f_time_index;
            real_t f_frequency_interval;
            count_t f_frequency_index;
    };

    template< >
    class ascii::pull< point_data >
    {
        public:
            pull( ascii& p_stream, point_data& p_data )
            {
                string_t t_hash;

                count_t t_size;
                real_t t_time_interval;
                count_t t_time_index;
                real_t t_frequency_interval;
                count_t t_frequency_index;

                p_stream >> t_hash >> t_size;
                p_stream >> t_hash >> t_time_interval;
                p_stream >> t_hash >> t_time_index;
                p_stream >> t_hash >> t_frequency_interval;
                p_stream >> t_hash >> t_frequency_index;

                p_data.set_size( t_size );
                p_data.set_time_interval( t_time_interval );
                p_data.set_time_index( t_time_index );
                p_data.set_frequency_interval( t_frequency_interval );
                p_data.set_frequency_index( t_frequency_index );
                for( index_t t_index = 0; t_index < t_size; t_index++ )
                {
                    p_stream >> p_data.at( t_index );
                }
            }
    };
    template< >
    class ascii::push< point_data >
    {
        public:
            push( ascii& p_stream, point_data& p_data )
            {
                p_stream << "# " << p_data.get_size() << "\n";
                p_stream << "# " << p_data.get_time_interval() << "\n";
                p_stream << "# " << p_data.get_time_index() << "\n";
                p_stream << "# " << p_data.get_frequency_interval() << "\n";
                p_stream << "# " << p_data.get_frequency_index() << "\n";
                for( index_t t_index = 0; t_index < p_data.get_size(); t_index++ )
                {
                    p_stream << p_data.at( t_index );
                }
                p_stream << "\n";
            }
    };

    template< >
    class binary::pull< point_data >
    {
        public:
            pull( binary& p_stream, point_data& p_data )
            {
                count_t t_size;
                real_t t_time_interval;
                count_t t_time_index;
                real_t t_frequency_interval;
                count_t t_frequency_index;

                p_stream >> t_size;
                p_stream >> t_time_interval;
                p_stream >> t_time_index;
                p_stream >> t_frequency_interval;
                p_stream >> t_frequency_index;

                p_data.set_size( t_size );
                p_data.set_time_interval( t_time_interval );
                p_data.set_time_index( t_time_index );
                p_data.set_frequency_interval( t_frequency_interval );
                p_data.set_frequency_index( t_frequency_index );
                for( index_t t_index = 0; t_index < p_data.get_size(); t_index++ )
                {
                    p_stream >> p_data.at( t_index );
                }
            }
    };
    template< >
    class binary::push< point_data >
    {
        public:
            push( binary& p_stream, point_data& p_data )
            {
                p_stream << p_data.get_size();
                p_stream << p_data.get_time_interval();
                p_stream << p_data.get_time_index();
                p_stream << p_data.get_frequency_interval();
                p_stream << p_data.get_frequency_index();
                for( index_t t_index = 0; t_index < p_data.get_size(); t_index++ )
                {
                    p_stream << p_data.at( t_index );
                }
            }
    };

}

#endif
