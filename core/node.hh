#ifndef _midge_node_hh_
#define _midge_node_hh_

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
            virtual void initialize( node* p_caller ) = 0;
            virtual void deinitialize( node* p_caller ) = 0;
            virtual void update( node* p_caller ) = 0;
            virtual void deupdate( node* p_caller ) = 0;
    };

}

#endif
