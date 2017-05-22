#include "message_logger.hh"

message_declare( testmsg );
message_define( testmsg, test, test )

using namespace midge;

int main()
{
    msg_normal( testmsg, "hello" );
    msg_warning( testmsg, "uh oh..." );
    msg_error( testmsg, "yikes! get ready for a stack trace!" );

    return 0;
}
