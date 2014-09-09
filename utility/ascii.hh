#ifndef _midge_file_hh_
#define _midge_file_hh_

#include "types.hh"

#include <fstream>
using std::fstream;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <map>
using std::map;

#include <utility>
using std::pair;

namespace midge
{

    class ascii
    {
        public:
            ascii();
            virtual ~ascii();

        public:
            void read( const string& p_file );
            void write( const string& p_file );
            void close();
            bool end();

        public:
            template< class x_type >
            ascii& operator>>( x_type& p_object )
            {
                pull< x_type >( *this, p_object );
                return *this;
            }

            template< class x_type >
            ascii& operator<<( const x_type& p_object )
            {
                push< x_type >( *this, p_object );
                return *this;
            }

        private:
            template< class x_type >
            class pull
            {
                public:
                    pull( ascii& p_stream, x_type& p_object )
                    {
                        p_stream.f_fstream >> p_object;
                    }
            };
            template< class x_type >
            class push
            {
                public:
                    push( ascii& p_stream, const x_type& p_object )
                    {
                        p_stream.f_fstream << p_object;
                    }
            };

            template< class x_type, int x_size >
            class pull< x_type[ x_size ] >
            {
                public:
                    pull( ascii& p_stream, x_type p_object[ x_size ] )
                    {
                        for( index_t t_index = 0; t_index < x_size; t_index++ )
                        {
                            p_stream.f_fstream >> p_object[ t_index ];
                        }
                    }
            };
            template< class x_type, int x_size >
            class push< x_type[ x_size ] >
            {
                public:
                    push( ascii& p_stream, const x_type p_object[ x_size ] )
                    {
                        index_t t_index = 0;

                        if( t_index != x_size )
                        {
                            p_stream << p_object[ t_index ];
                            t_index++;
                            while( t_index < x_size )
                            {
                                p_stream << " " << p_object[ t_index ];
                                t_index++;
                            }
                        }
                    }
            };

            template< class x_value >
            class pull< vector< x_value > >
            {
                public:
                    pull( ascii& p_stream, vector< x_value >& p_vector )
                    {
                        count_t t_size;
                        p_stream >> t_size;
                        p_vector.resize( t_size );
                        for( typename vector< x_value >::iterator t_it = p_vector.begin(); t_it != p_vector.end(); t_it++ )
                        {
                            p_stream >> (*t_it);
                        }
                    }
            };
            template< class x_value >
            class push< vector< x_value > >
            {
                public:
                    push( ascii& p_stream, const vector< x_value >& p_vector )
                    {
                        const count_t t_size = p_vector.size();
                        p_stream << t_size << " ";

                        typename vector< x_value >::const_iterator t_it = p_vector.begin();
                        if( t_it != p_vector.end() )
                        {
                            p_stream << (*t_it);
                            t_it++;
                            while( t_it != p_vector.end() )
                            {
                                p_stream << " " << (*t_it);
                                t_it++;
                            }
                        }
                    }
            };

            template< class x_key, class x_value >
            class pull< map< x_key, x_value > >
            {
                public:
                    pull( ascii& p_stream, map< x_key, x_value >& p_map )
                    {
                        count_t t_size;
                        p_stream >> t_size;
                        x_key t_key;
                        x_value t_value;
                        p_map.clear();
                        for( index_t t_index = 0; t_index < t_size; t_index++ )
                        {
                            p_stream >> t_key;
                            p_stream >> t_value;
                            p_map.insert( pair< x_key, x_value >( t_key, t_value ) );
                        }
                    }
            };
            template< class x_key, class x_value >
            class push< map< x_key, x_value > >
            {
                public:
                    push( ascii& p_stream, const map< x_key, x_value >& p_map )
                    {
                        const count_t t_size = p_map.size();
                        p_stream << t_size << " ";

                        typename map< x_key, x_value >::const_iterator t_it = p_map.begin();
                        if( t_it != p_map.end() )
                        {
                            p_stream << t_it->first << " " << t_it->second;
                            t_it++;
                            while( t_it != p_map.end() )
                            {
                                p_stream << " " << t_it->first << " " << t_it->second;
                                t_it++;
                            }
                        }
                    }
            };

            template< int x_size >
            class push< char[ x_size ] >
            {
                public:
                    push( ascii& p_stream, const char p_data[ x_size ] )
                    {
                        p_stream.f_fstream << p_data;
                    }
            };

        private:
            fstream f_fstream;
    };

}

#endif
