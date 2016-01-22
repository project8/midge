#include "compiler.hh"
#include "lexer.hh"
#include "object_builder.hh"

#include <iostream>

using namespace midge;

using std::cout;
using std::endl;

using std::string;
using std::vector;

class child
{
    public:
        child() :
                f_first( "" ),
                f_second( 0 )
        {
        }
        ~child()
        {
            cout << "  child:" << endl;
            cout << "    first: " << f_first << endl;
            cout << "    second: " << f_second << endl;
        }

    public:
        void first( const string& p_string )
        {
            f_first = p_string;
            return;
        }
        void second( const real_t& p_real_t )
        {
            f_second = p_real_t;
            return;
        }

    private:
        string f_first;
        real_t f_second;
};

typedef object_builder< child > child_builder;

static const int s_child =
    child_builder::lingual< string >( &child::first, "first" ) +
    child_builder::numerical< real_t >( &child::second, "second" );

class parent
{
    public:
        parent() :
                f_children()
        {
        }
        ~parent()
        {
            cout << "parent:" << endl;
            for( vector< child* >::iterator t_it = f_children.begin(); t_it != f_children.end(); t_it++ )
            {
                delete (*t_it);
            }
        }

    public:
        void add( child* p_child )
        {
            f_children.push_back( p_child );
            return;
        }

    private:
        vector< child* > f_children;
};

typedef object_builder< parent > parent_builder;

static const int s_parent =
    parent_builder::object< child >( &parent::add, "add" );

int main( int p_count, char** p_values )
{
    if( p_count < 2 )
    {
        cout << "usage:" << endl;
        cout << "  test_builder <input filename>" << endl;
        return (-1);
    }

    string t_input_file( p_values[ 1 ] );

    lexer t_lexer;
    compiler t_compiler;

    t_compiler.insert_after( &t_lexer );

    try
    {
        t_lexer( t_input_file );
    }
    catch( const error& t_error )
    {
        cout << "an error occurred:" << endl;
        cout << "  " << t_error.what() << endl;
        return -1;
    }
    catch( ... )
    {
        cout << "an unknown error occurred" << endl;
        return -1;
    }

    parent_builder t_builder;
    t_builder( t_compiler() );
    delete (t_builder());

    return 0;
}
