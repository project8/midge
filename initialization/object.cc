#include "object.hh"

#include "error.hh"

namespace midge
{

    object::object() :
            f_vector(),
            f_multimap()
    {
        set( this );
    }
    object::~object()
    {
        for( vector_it_t t_it = f_vector.begin(); t_it != f_vector.end(); t_it++ )
        {
            delete t_it->second;
        }
    }
    object* object::clone() const
    {
        object* t_clone = new object();
        for( vector_cit_t t_it = f_vector.begin(); t_it != f_vector.end(); t_it++ )
        {
            t_clone->add( t_it->first, t_it->second->clone() );
        }
        return t_clone;
    }

    count_t object::size() const
    {
        return f_vector.size();
    }
    void object::add( const string& p_key, value* p_value )
    {
        f_vector.push_back( vector_entry_t( p_key, p_value ) );
        f_multimap.insert( multimap_entry_t( p_key, p_value ) );
        return;
    }

    pair< string, value* > object::at( const count_t& p_index )
    {
        string t_key;
        value* t_value;
        if( p_index < f_vector.size() )
        {
            t_key = f_vector.at( p_index ).first;
            t_value = f_vector.at( p_index ).second;
            return (pair< string, value* >( t_key, t_value ));
        }

        throw error() << "object has no value for index <" << p_index << ">";
        return (pair< string, value* >( "", NULL ));
    }
    pair< string, const value* > object::at( const count_t& p_index ) const
    {
        string t_key;
        value* t_value;
        if( p_index < f_vector.size() )
        {
            t_key = f_vector.at( p_index ).first;
            t_value = f_vector.at( p_index ).second;
            return (pair< string, const value* >( t_key, t_value ));
        }

        throw error() << "object has no value for index <" << p_index << ">";
        return (pair< string, const value* >( "", NULL ));
    }

    value* object::at( const string& p_key, const count_t& p_index )
    {
        count_t t_index = p_index;
        multimap_it_t t_first = f_multimap.lower_bound( p_key );
        multimap_it_t t_last = f_multimap.upper_bound( p_key );
        if( t_first != f_multimap.end() )
        {
            for( multimap_it_t t_it = t_first; t_it != t_last; t_it++ )
            {
                if( t_index == 0 )
                {
                    return t_it->second;
                }
                t_index--;
            }
            throw error() << "object has no value for key <" << p_key << "> at index <" << p_index << ">";
            return NULL;
        }
        throw error() << "object has no value for key <" << p_key << ">";
        return NULL;
    }
    const value* object::at( const string& p_key, const count_t& p_index ) const
    {
        count_t t_index = p_index;
        multimap_cit_t t_first = f_multimap.lower_bound( p_key );
        multimap_cit_t t_last = f_multimap.upper_bound( p_key );
        if( t_first != f_multimap.end() )
        {
            for( multimap_cit_t t_it = t_first; t_it != t_last; t_it++ )
            {
                if( t_index == 0 )
                {
                    return t_it->second;
                }
                t_index--;
            }
            throw error() << "object has no value for key <" << p_key << "> at index <" << p_index << ">";
            return NULL;
        }
        throw error() << "object has no value for key <" << p_key << ">";
        return NULL;
    }

}
