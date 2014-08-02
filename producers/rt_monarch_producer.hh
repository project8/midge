#ifndef _midge_rt_monarch_producer_hh_
#define _midge_rt_monarch_producer_hh_

#include "producer.hh"
#include "rt_data.hh"
#include "typelist.hh"

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
            void set_file( const string& p_filename );
            const string& get_file() const;

            void set_stride( const count_t& p_stride );
            const count_t& get_stride() const;

            void set_size( const count_t& p_size );
            const count_t& get_size() const;

        private:
            string f_file;
            count_t f_stride;
            count_t f_size;

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

            real_t* f_out;
            count_t f_index;
            count_t f_begin;
            count_t f_end;

        protected:
            void initialize_producer();
            bool start_producer();
            bool execute_producer();
            bool stop_producer();
            void finalize_producer();
    };

}

#endif
