#include "../library/initialization/processor.hh"

namespace midge
{

    processor::processor() :
            f_parent( NULL ),
            f_child( NULL )
    {
    }
    processor::~processor()
    {
    }

    void processor::connect( processor* p_parent, processor* p_child )
    {
        if( (p_parent == NULL) || (p_child == NULL) )
        {
            return;
        }

        if( (p_parent->f_child != NULL) || (p_child->f_parent != NULL) )
        {
            return;
        }

        p_parent->f_child = p_child;
        p_child->f_parent = p_parent;

        return;
    }
    void processor::disconnect( processor* p_parent, processor* p_child )
    {
        if( (p_parent == NULL) || (p_child == NULL) )
        {
            return;
        }

        if( (p_parent->f_child != p_child) || (p_child->f_parent != p_parent) )
        {
            return;
        }

        p_parent->f_child = NULL;
        p_child->f_parent = NULL;

        return;
    }

    void processor::insert_before( processor* p_target )
    {
        if( (f_parent != NULL) || (f_child != NULL) || (p_target == NULL) )
        {
            return;
        }

        if( p_target->f_parent != NULL )
        {
            f_parent = p_target->f_parent;
            f_parent->f_child = this;
        }

        f_child = p_target;
        p_target->f_parent = this;

        return;
    }
    void processor::insert_after( processor* p_target )
    {
        if( (f_parent != NULL) || (f_child != NULL) || (p_target == NULL) )
        {
            return;
        }

        if( p_target->f_child != NULL )
        {
            f_child = p_target->f_child;
            f_child->f_parent = this;
        }

        f_parent = p_target;
        p_target->f_child = this;

        return;
    }
    void processor::remove()
    {
        if( (f_parent != NULL) && (f_child != NULL) )
        {
            f_parent->f_child = f_child;
            f_child->f_parent = f_parent;

            f_parent = NULL;
            f_child = NULL;

            return;
        }

        if( f_parent != NULL )
        {
            f_parent->f_child = NULL;
            f_parent = NULL;
        }

        if( f_child != NULL )
        {
            f_child->f_parent = NULL;
            f_child = NULL;
        }

        return;
    }

    processor* processor::get_first_parent()
    {
        if( f_parent != NULL )
        {
            return f_parent->get_first_parent();
        }
        return this;
    }
    processor* processor::get_parent()
    {
        return f_parent;
    }

    processor* processor::get_last_child()
    {
        if( f_child != NULL )
        {
            return f_child->get_last_child();
        }
        return this;
    }
    processor* processor::get_child()
    {
        return f_child;
    }

    void processor::process_key( string p_string )
    {
        if( f_child == NULL )
        {
            return;
        }
        else
        {
            f_child->process_key( p_string );
            return;
        }
    }
    void processor::process_lingual( string p_string )
    {
        if( f_child == NULL )
        {
            return;
        }
        else
        {
            f_child->process_lingual( p_string );
            return;
        }
    }
    void processor::process_numerical( string p_string )
    {
        if( f_child == NULL )
        {
            return;
        }
        else
        {
            f_child->process_numerical( p_string );
            return;
        }
    }
    void processor::process_boolean( string p_string )
    {
        if( f_child == NULL )
        {
            return;
        }
        else
        {
            f_child->process_boolean( p_string );
            return;
        }
    }
    void processor::process_null()
    {
        if( f_child == NULL )
        {
            return;
        }
        else
        {
            f_child->process_null();
            return;
        }
    }
    void processor::process_object_start()
    {
        if( f_child == NULL )
        {
            return;
        }
        else
        {
            f_child->process_object_start();
            return;
        }
    }
    void processor::process_object_stop()
    {
        if( f_child == NULL )
        {
            return;
        }
        else
        {
            f_child->process_object_stop();
            return;
        }
    }
    void processor::process_array_start()
    {
        if( f_child == NULL )
        {
            return;
        }
        else
        {
            f_child->process_array_start();
            return;
        }
    }
    void processor::process_array_stop()
    {
        if( f_child == NULL )
        {
            return;
        }
        else
        {
            f_child->process_array_stop();
            return;
        }
    }
    void processor::process_start()
    {
        if( f_child == NULL )
        {
            return;
        }
        else
        {
            f_child->process_start();
            return;
        }
    }
    void processor::process_stop()
    {
        if( f_child == NULL )
        {
            return;
        }
        else
        {
            f_child->process_stop();
            return;
        }
    }

}
