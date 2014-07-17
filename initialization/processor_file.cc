#include "processor_file.hh"

#include <iostream>
using std::cout;
using std::endl;

namespace midge
{

    processor_file::processor_file() :
            f_pre( "" ),
            f_post( "" )
    {
    }
    processor_file::~processor_file()
    {
    }

    void processor_file::process_key( token* p_token )
    {
        cout << "  \"" << p_token->as< string >() << "\" : ";
        return;
    }
    void processor_file::process_string( token* p_token )
    {
        cout << "\"" << p_token->as< string >() << "\"" << f_post << endl << f_pre;
        return;
    }
    void processor_file::process_boolean( token* p_token )
    {
        cout << (p_token->as< bool >() == true ? "true" : "false") << f_post << endl << f_pre;
        return;
    }
    void processor_file::process_numerical( token* p_token )
    {
        cout << p_token->as< double >() << f_post << endl << f_pre;
        return;
    }
    void processor_file::process_object_start()
    {
        cout << "{" << endl;
        f_pre.assign( " ", f_pre.length() + 2 );
        f_post.assign( ",", 1 );
        return;
    }
    void processor_file::process_object_stop()
    {
        cout << "{" << endl;
        f_pre.assign( " ", f_pre.length() - 2 );
        f_post.assign( ",", 0 );
        return;
    }
    void processor_file::process_array_start()
    {
        cout << "[" << endl;
        f_pre.assign( " ", f_pre.length() + 2 );
        f_post.assign( ",", 1 );
        return;
    }
    void processor_file::process_array_stop()
    {
        cout << "]" << endl;
        f_pre.assign( " ", f_pre.length() - 2 );
        f_post.assign( ",", 0 );
    }
    void processor_file::process_start()
    {
        cout << "parsing begun..." << endl;
        return;
    }
    void processor_file::process_stop()
    {
        cout << "...parsing complete" << endl;
    }

}
