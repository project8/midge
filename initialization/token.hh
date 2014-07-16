#ifndef TOKEN_HH_
#define TOKEN_HH_

#include <string>
using std::string;

namespace midge
{
    namespace initialization
    {

        class token
        {
            public:
                token();
                virtual ~token();

            private:
                std::string f_value;
        };

    }
}

#endif
