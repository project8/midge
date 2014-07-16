#ifndef _midge_link_hh_
#define _midge_link_hh_

#include <string>
using std::string;

namespace midge
{

    class node;

    class link
    {
        public:
            link();
            virtual ~link();

        public:
            void set_name( const string& p_name );
            const string& get_name() const;

        private:
            string f_name;

        public:
            virtual void connect( node* p_node ) = 0;
    };

}

#endif
