#include "processor_terminal.hh"

#include <iostream>
using std::cout;
using std::endl;

namespace midge
{

    processor_terminal::processor_terminal()
    {
    }
    processor_terminal::~processor_terminal()
    {
    }

    void processor_terminal::process_key( token* p_token )
    {
        cout << "[key] " << p_token->str() << endl;
        return;
    }
    void processor_terminal::process_string( token* p_token )
    {
        cout << "[string] " << p_token->str() << endl;
        return;
    }
    void processor_terminal::process_boolean( token* p_token )
    {
        cout << "[bool] " << p_token->str() << endl;
        return;
    }
    void processor_terminal::process_numerical( token* p_token )
    {
        cout << "[double] " << p_token->str() << endl;
        return;
    }
    void processor_terminal::process_object_start()
    {
        cout << "[object start]" << endl;
        return;
    }
    void processor_terminal::process_object_stop()
    {
        cout << "[object stop]" << endl;
        return;
    }
    void processor_terminal::process_array_start()
    {
        cout << "[array start]" << endl;
        return;
    }
    void processor_terminal::process_array_stop()
    {
        cout << "[array stop]" << endl;
        return;
    }
    void processor_terminal::process_start()
    {
        cout << "parsing begun..." << endl;
        return;
    }
    void processor_terminal::process_stop()
    {
        cout << "...parsing complete" << endl;
    }

}
