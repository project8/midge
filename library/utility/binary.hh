#ifndef _midge_binary_hh_
#define _midge_binary_hh_

#include <fstream>

#include "types.hh"

#include <string>
#include <vector>
#include <list>
#include <map>
#include <utility>

namespace midge
{

    class binary
    {
        public:
            binary();
            virtual ~binary();

        public:
            void read( const std::string& p_file );
            void write( const std::string& p_file );
            void close();
            bool end();

        public:
            template< class x_type >
            binary& operator>>( x_type& p_object )
            {
                pull< x_type >( *this, p_object );
                return *this;
            }

            template< class x_type >
            binary& operator<<( const x_type& p_object )
            {
                push< x_type >( *this, p_object );
                return *this;
            }

        private:
            template< class x_type >
            class pull
            {
                public:
                    pull( binary& p_stream, x_type& p_object )
                    {
                        p_stream.f_fstream.read( reinterpret_cast< char* >( &p_object ), sizeof(x_type) );
                    }
            };
            template< class x_type >
            class push
            {
                public:
                    push( binary& p_stream, const x_type& p_object )
                    {
                        p_stream.f_fstream.write( reinterpret_cast< const char* >( &p_object ), sizeof(x_type) );
                    }
            };

            template< class x_value >
            class pull< std::vector< x_value > >
            {
                public:
                    pull( binary& p_stream, std::vector< x_value >& p_vector )
                    {
                        count_t t_size;
                        p_stream >> t_size;
                        p_vector.resize( t_size );
                        for( typename std::vector< x_value >::iterator t_it = p_vector.begin(); t_it != p_vector.end(); t_it++ )
                        {
                            p_stream >> (*t_it);
                        }
                    }
            };
            template< class x_value >
            class push< std::vector< x_value > >
            {
                public:
                    push( binary& p_stream, const std::vector< x_value >& p_vector )
                    {
                        p_stream << p_vector.size();
                        for( typename std::vector< x_value >::const_iterator t_it = p_vector.begin(); t_it != p_vector.end(); t_it++ )
                        {
                            p_stream << (*t_it);
                        }
                    }
            };

            template< class x_value >
            class pull< std::list< x_value > >
            {
                public:
                    pull( binary& p_stream, std::list< x_value >& p_list )
                    {
                        count_t t_size;
                        p_stream >> t_size;
                        p_list.resize( t_size );
                        for( typename std::list< x_value >::iterator t_it = p_list.begin(); t_it != p_list.end(); t_it++ )
                        {
                            p_stream >> (*t_it);
                        }
                    }
            };
            template< class x_value >
            class push< std::list< x_value > >
            {
                public:
                    push( binary& p_stream, const std::list< x_value >& p_list )
                    {
                        p_stream << p_list.size();
                        for( typename std::list< x_value >::const_iterator t_it = p_list.begin(); t_it != p_list.end(); t_it++ )
                        {
                            p_stream << (*t_it);
                        }
                    }
            };

            template< class x_key, class x_value >
            class pull< std::map< x_key, x_value > >
            {
                public:
                    pull( binary& p_stream, std::map< x_key, x_value >& p_map )
                    {
                        count_t t_size;
                        p_stream >> t_size;
                        x_key t_key;
                        x_value t_value;
                        p_map.clear();
                        for( count_t t_index = 0; t_index < t_size; t_index++ )
                        {
                            p_stream >> t_key >> t_value;
                            p_map.insert( std::pair< x_key, x_value >( t_key, t_value ) );
                        }
                    }
            };
            template< class x_key, class x_value >
            class push< std::map< x_key, x_value > >
            {
                public:
                    push( binary& p_stream, const std::map< x_key, x_value >& p_map )
                    {
                        p_stream << p_map.size();
                        for( typename std::map< x_key, x_value >::const_iterator t_it = p_map.begin(); t_it != p_map.end(); t_it++ )
                        {
                            p_stream << t_it->first << t_it->second;
                        }
                    }
            };

        private:
            std::fstream f_fstream;
    };

    template< >
    class binary::pull< std::string >
    {
        public:
            pull( binary& p_stream, std::string& p_string )
            {
                size_t t_size;
                p_stream.f_fstream.read( reinterpret_cast< char* >( &t_size ), sizeof(size_t) );
                p_string.resize( t_size );
                p_stream.f_fstream.read( &p_string[ 0 ], t_size );
            }
    };

    template< >
    class binary::push< std::string >
    {
        public:
            push( binary& p_stream, const std::string& p_string )
            {
                const size_t t_size = p_string.size();
                p_stream.f_fstream.write( reinterpret_cast< const char* >( &t_size ), sizeof(size_t) );
                p_stream.f_fstream.write( p_string.c_str(), t_size );
            }
    };

}

#endif
