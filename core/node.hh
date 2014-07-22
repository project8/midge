#ifndef _midge_node_hh_
#define _midge_node_hh_

#include "link.hh"

#include <map>
using std::map;

#include <string>
using std::string;

namespace midge
{

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
            virtual void initialize() = 0;
            virtual void start() = 0;
            virtual void execute() = 0;
            virtual void stop() = 0;
            virtual void finalize() = 0;

        public:
            link* in( const string& p_label );
            link* out( const string& p_label );

        protected:
            template< class x_target, class x_argument >
            void in( void (x_target::*p_member)( x_argument* ), const string& p_label );
            void in( node* p_node, const string& p_link, const string& p_label );

            template< class x_target, class x_argument >
            void out( void (x_target::*p_member)( x_argument* ), const string& p_label );
            void out( node* p_target, const string& p_link, const string& p_label );

        private:
            typedef map< string, link* > link_map;
            typedef link_map::iterator link_it;
            typedef link_map::const_iterator link_cit;
            typedef link_map::value_type link_entry;

            link_map f_in_map;
            link_map f_out_map;
    };

}

#include "member.hh"

namespace midge
{

    template< class x_target, class x_argument >
    inline void node::in( void (x_target::*p_member)( x_argument* ), const string& p_label )
    {
        link_it t_it = f_in_map.find( p_label );
        if( t_it == f_in_map.end() )
        {
            link* t_in = new member< x_target, x_argument >( p_member );
            t_in->set_target( this );
            t_in->set_name( p_label );
            f_in_map.insert( link_entry( p_label, t_in ) );
        }
        else
        {
            throw error() << "node already has in named <" << p_label << ">";
        }
        return;
    }

    inline void node::in( node* p_target, const string& p_link, const string& p_label )
    {
        link* t_link = p_target->in( p_link );
        if( t_link != NULL )
        {
            link_it t_it = f_in_map.find( p_label );
            if( t_it == f_in_map.end() )
            {
                link* t_in = t_link->clone();
                t_in->set_name( p_label );
                f_in_map.insert( link_entry( p_label, t_in ) );
            }
            else
            {
                throw error() << "node already has in named <" << p_label << ">";
            }
        }
        else
        {
            throw error() << "node already has no in named <" << p_link << ">";
        }
        return;
    }

    template< class x_target, class x_argument >
    inline void node::out( void (x_target::*p_member)( x_argument* ), const string& p_label )
    {
        link_it t_it = f_out_map.find( p_label );
        if( t_it == f_out_map.end() )
        {
            link* t_out = new member< x_target, x_argument >( p_member );
            t_out->set_target( this );
            t_out->set_name( p_label );
            f_out_map.insert( link_entry( p_label, t_out ) );
        }
        else
        {
            throw error() << "node already has out named <" << p_label << ">";
        }
        return;
    }

    inline void node::out( node* p_target, const string& p_link, const string& p_label )
    {
        link* t_link = p_target->out( p_link );
        if( t_link != NULL )
        {
            link_it t_it = f_out_map.find( p_label );
            if( t_it == f_out_map.end() )
            {
                link* t_out = t_link->clone();
                t_out->set_name( p_label );
                f_out_map.insert( link_entry( p_label, t_out ) );
            }
            else
            {
                throw error() << "node already has out named <" << p_label << ">";
            }
        }
        else
        {
            throw error() << "node already has no out named <" << p_link << ">";
        }
        return;
    }

}

#endif
