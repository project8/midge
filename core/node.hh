#ifndef _midge_node_hh_
#define _midge_node_hh_

#include "error.hh"

#include <string>
using std::string;

namespace midge
{

    class link;

    class node
    {
        public:
            node();
            virtual ~node();

        public:
            void set_name( const string& p_name );
            const string& get_name() const;

        private:
            string f_name;

        public:
            virtual link* input( const string& p_label ) = 0;
            virtual link* output( const string& p_label ) = 0;

        public:
            virtual void initialize() = 0;
            virtual void execute() = 0;
            virtual void finalize() = 0;
    };

}

#endif
