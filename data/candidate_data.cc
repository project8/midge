#include "candidate_data.hh"

namespace midge
{

    candidate::candidate() :
            f_candidate_score( -1. ),
            f_cluster_score( -1. ),
            f_line_score( -1. )
    {
    }
    candidate::~candidate()
    {
    }

    candidate_data::candidate_data() :
            f_time_interval( 1. ),
            f_time_index( 0 ),
            f_frequency_interval( 1. ),
            f_frequency_index( 0 ),
            f_data(),
            f_size( 0 )
    {
    }
    candidate_data::~candidate_data()
    {
    }

    command_t candidate_data::command()
    {
        return stream::s_none;
    }
    void candidate_data::command( command_t )
    {
        return;
    }
    candidate_data& candidate_data::operator>>( candidate_data& p_data )
    {

        p_data.f_time_interval = f_time_interval;
        p_data.f_time_index = f_time_index;
        p_data.f_frequency_interval = f_frequency_interval;
        p_data.f_frequency_index = f_frequency_index;
        p_data.set_size( get_size() );
        for( count_t t_index = 0; t_index < get_size(); t_index++ )
        {
            p_data[ t_index ] = (*this)[ t_index ];
        }
        return *this;
    }
    candidate_data& candidate_data::operator<<( const candidate_data& p_data )
    {
        f_time_interval = p_data.f_time_interval;
        f_time_index = p_data.f_time_index;
        f_frequency_interval = p_data.f_frequency_interval;
        f_frequency_index = p_data.f_frequency_index;
        set_size( p_data.get_size() );
        for( count_t t_index = 0; t_index < p_data.get_size(); t_index++ )
        {
            (*this)[ t_index ] = p_data[ t_index ];
        }
        return *this;
    }

    const candidate& candidate_data::operator[]( const count_t& p_index ) const
    {
        return f_data[ p_index ];
    }
    candidate& candidate_data::operator[]( const count_t& p_index )
    {
        return f_data[ p_index ];
    }

    void candidate_data::set_size( const count_t& p_size )
    {
        if( p_size == f_size )
        {
            return;
        }

        f_data.resize( p_size );
        f_size = p_size;

        return;
    }
    const count_t& candidate_data::get_size() const
    {
        return f_size;
    }

}
