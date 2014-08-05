#ifndef _midge_rt_monarch_producer_hh_
#define _midge_rt_monarch_producer_hh_

#include "producer.hh"
#include "rt_data.hh"
#include "typelist.hh"
#include "macros.hh"

#include "Monarch.hpp"

namespace midge
{

    class rt_monarch_producer :
        public _producer< rt_monarch_producer, typelist_1( rt_data )>
    {
        public:
            rt_monarch_producer();
            virtual ~rt_monarch_producer();

        public:
            accessible( string, file )
            accessible( real_t, minimum_time )
            accessible( real_t, maximum_time )
            accessible( count_t, stride )
            accessible( count_t, size )

        private:
            const monarch::Monarch* f_monarch;
            const monarch::MonarchHeader* f_header;
            const monarch::MonarchRecord< monarch::DataType >* f_record;

            count_t f_length;
            real_t f_interval;
            real_t f_voltage_minimum;
            real_t f_voltage_range;
            real_t f_voltage_inverse_range;
            real_t f_voltage_levels;
            real_t f_voltage_inverse_levels;

            count_t f_minimum_index;
            count_t f_maximum_index;

            real_t* f_out;
            count_t f_index;
            count_t f_next;
            count_t f_samples;

        protected:
            void initialize_producer();
            bool start_producer();
            bool execute_producer();
            bool stop_producer();
            void finalize_producer();
    };

}

#endif
