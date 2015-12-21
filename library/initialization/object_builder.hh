#ifndef _midge_object_builder_hh_
#define _midge_object_builder_hh_

#include <map>

#include "../library/initialization/boolean_builder.hh"
#include "../library/initialization/lingual_builder.hh"
#include "../library/initialization/null_builder.hh"
#include "../library/initialization/numerical_builder.hh"
#include "../library/initialization/object.hh"
#include "../library/initialization/value.hh"
#include "../utility/typelist.hh"
using std::map;

namespace midge
{

    template< class x_type >
    class object_builder
    {
        public:
            object_builder()
            {
                if( f_entries == NULL )
                {
                    f_entries = new map_t();
                }

                f_type = new x_type();
            }
            virtual ~object_builder()
            {
                delete f_type;
            }

        public:
            void operator()( value* p_value )
            {
                if( p_value->is< ::midge::object >() == true )
                {
                    operator()( p_value->as< ::midge::object >() );
                }
                else
                {
                    throw error() << "object builder for <" << typeid(x_type).name() << "> received unknown value type";
                }
                return;
            }
            void operator()( ::midge::object* p_object )
            {
                pair< string, value* > t_pair;
                string t_label;
                value* t_value;

                map_it_t t_it;

                for( index_t t_index = 0; t_index < p_object->size(); t_index++ )
                {
                    t_pair = p_object->at( t_index );
                    t_label = t_pair.first;
                    t_value = t_pair.second;

                    t_it = f_entries->find( t_label );

                    if( t_it != f_entries->end() )
                    {
                        t_it->second->bind( f_type, t_value );
                    }
                    else
                    {
                        throw error() << "object builder for <" << typeid(x_type).name() << "> has no entry for label <" << t_label << ">";
                    }
                }
            }

            x_type* operator()()
            {
                x_type* t_type = f_type;
                f_type = NULL;
                return t_type;
            }

        private:
            x_type* f_type;

        public:
            template< class x_child, class x_member >
            static int object( x_member p_member, const string& p_label )
            {
                if( f_entries == NULL )
                {
                    f_entries = new map< string, entry* >();
                }

                map_it_t t_it = f_entries->find( p_label );
                if( t_it == f_entries->end() )
                {
                    f_entries->insert( map_entry_t( p_label, new member< ::midge::object_builder, x_child, x_member >( p_member ) ) );
                }
                else
                {
                    throw error() << "object builder for <" << typeid(x_type).name() << "> already has entry for label <" << p_label << ">";
                }

                return 0;
            }

            template< class x_child, class x_member >
            static int lingual( x_member p_member, const string& p_label )
            {
                if( f_entries == NULL )
                {
                    f_entries = new map< string, entry* >();
                }

                map_it_t t_it = f_entries->find( p_label );
                if( t_it == f_entries->end() )
                {
                    f_entries->insert( map_entry_t( p_label, new member< ::midge::lingual_builder, x_child, x_member >( p_member ) ) );
                }
                else
                {
                    throw error() << "object builder for <" << typeid(x_type).name() << "> already has entry for label <" << p_label << ">";
                }

                return 0;
            }

            template< class x_child, class x_member >
            static int numerical( x_member p_member, const string& p_label )
            {
                if( f_entries == NULL )
                {
                    f_entries = new map< string, entry* >();
                }

                map_it_t t_it = f_entries->find( p_label );
                if( t_it == f_entries->end() )
                {
                    f_entries->insert( map_entry_t( p_label, new member< ::midge::numerical_builder, x_child, x_member >( p_member ) ) );
                }
                else
                {
                    throw error() << "object builder for <" << typeid(x_type).name() << "> already has entry for label <" << p_label << ">";
                }

                return 0;
            }

            template< class x_child, class x_member >
            static int boolean( x_member p_member, const string& p_label )
            {
                if( f_entries == NULL )
                {
                    f_entries = new map< string, entry* >();
                }

                map_it_t t_it = f_entries->find( p_label );
                if( t_it == f_entries->end() )
                {
                    f_entries->insert( map_entry_t( p_label, new member< ::midge::boolean_builder, x_child, x_member >( p_member ) ) );
                }
                else
                {
                    throw error() << "object builder for <" << typeid(x_type).name() << "> already has entry for label <" << p_label << ">";
                }

                return 0;
            }

            template< class x_child, class x_member >
            static int null( x_member p_member, const string& p_label )
            {
                if( f_entries == NULL )
                {
                    f_entries = new map< string, entry* >();
                }

                map_it_t t_it = f_entries->find( p_label );
                if( t_it == f_entries->end() )
                {
                    f_entries->insert( map_entry_t( p_label, new member< ::midge::null_builder, x_child, x_member >( p_member ) ) );
                }
                else
                {
                    throw error() << "object builder for <" << typeid(x_type).name() << "> already has entry for label <" << p_label << ">";
                }

                return 0;
            }

        private:
            class entry
            {
                public:
                    entry()
                    {
                    }
                    virtual ~entry()
                    {
                    }

                public:
                    virtual void bind( x_type* p_type, value* p_value ) = 0;
            };

            template< template< class > class x_builder, class x_built, class x_member >
            class member;

            template< template< class > class x_builder, class x_built, class x_target, class x_argument >
            class member< x_builder, x_built, void (x_target::*)( x_argument ) > :
                public entry
            {
                public:
                    member( void (x_target::*p_member)( x_argument ) ) :
                            f_member( p_member )
                    {
                    }
                    virtual ~member()
                    {
                    }

                public:
                    void bind( x_type* p_type, value* p_value )
                    {
                        x_builder< x_built > t_builder;
                        t_builder( p_value );
                        (p_type->*f_member)( *(t_builder()) );
                        return;
                    }

                private:
                    void (x_target::*f_member)( x_argument );
            };

            template< template< class > class x_builder, class x_built, class x_target, class x_argument >
            class member< x_builder, x_built, void (x_target::*)( x_argument* ) > :
                public entry
            {
                public:
                    member( void (x_target::*p_member)( x_argument* ) ) :
                            f_member( p_member )
                    {
                    }
                    virtual ~member()
                    {
                    }

                public:
                    void bind( x_type* p_type, value* p_value )
                    {
                        x_builder< x_built > t_builder;
                        t_builder( p_value );
                        (p_type->*f_member)( t_builder() );
                        return;
                    }

                private:
                    void (x_target::*f_member)( x_argument* );
            };

            typedef map< string, entry* > map_t;
            typedef typename map_t::iterator map_it_t;
            typedef typename map_t::const_iterator map_cit_t;
            typedef typename map_t::value_type map_entry_t;

            static map_t* f_entries;
    };

    template< class x_type >
    typename object_builder< x_type >::map_t* object_builder< x_type >::f_entries = NULL;

}

#endif
