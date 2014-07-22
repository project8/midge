#ifndef _midge_link_hh_
#define _midge_link_hh_

#include "error.hh"
#include "types.hh"

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
            virtual link* clone() const = 0;
            virtual void set_target( node* p_node ) = 0;
            virtual void set_argument( node* p_node ) = 0;
            virtual void connect() = 0;
    };

}

#endif
