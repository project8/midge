#ifndef _midge_count_controller_hh_
#define _midge_count_controller_hh_

#include "controller.hh"
#include "typelist.hh"

namespace midge
{

    class count_controller :
        public _controller< count_controller, typelist_0(),typelist_10( node, node, node, node, node, node, node, node, node, node ) >
    {
        public:
            count_controller();
            virtual ~count_controller();

        public:
            void set_count( const count_t& p_count );
            const count_t& get_count() const;

        private:
            count_t f_count;

            count_t f_current;

        protected:
            bool start_controller_pre();
            bool execute_controller_pre();
            bool execute_controller_post();
            bool stop_controller_post();
    };

}

#endif
