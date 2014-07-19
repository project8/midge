#include "array.hh"

#include "error.hh"

namespace midge
{

    array::array() :
            f_vector()
    {
        set( this );
    }
    array::~array()
    {
        for( vector_it_t t_it = f_vector.begin(); t_it != f_vector.end(); t_it++ )
        {
            delete *t_it;
        }
    }
    array* array::clone() const
    {
        array* t_clone = new array();
        for( vector_cit_t t_it = f_vector.begin(); t_it != f_vector.end(); t_it++ )
        {
            t_clone->add( (*t_it)->clone() );
        }
        return t_clone;
    }

    void array::add( value* p_value )
    {
        f_vector.push_back( vector_entry_t( p_value ) );
        return;
    }
    uint64_t array::size() const
    {
        return f_vector.size();
    }

    value* array::at( const uint64_t& p_index )
    {
        if( p_index < f_vector.size() )
        {
            return f_vector[ p_index ];
        }

        throw error() << "array has no value for index <" << p_index << ">";
        return this;
    }
    const value* array::at( const uint64_t& p_index ) const
    {
        if( p_index < f_vector.size() )
        {
            return f_vector[ p_index ];
        }

        throw error() << "array has no value for index <" << p_index << ">";
        return this;
    }

}
