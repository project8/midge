#include "../utility/message.hh"

message_declare( testmsg );
message_define( testmsg, test, test )

using namespace midge;

int main()
{
    testmsg( s_normal ) << "hello" << eom;
    testmsg( s_warning ) << "uh oh..." << eom;
    testmsg( s_error ) << "yikes! get ready for a stack trace!" << eom;

    return 0;
}
