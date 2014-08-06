#ifndef _midge_rt_monarch_consumer_hh_
#define _midge_rt_monarch_consumer_hh_

#include "consumer.hh"
#include "rt_data.hh"
#include "typelist.hh"

#include "Monarch.hpp"

namespace midge
{

    class rt_monarch_consumer :
        public _consumer< rt_monarch_consumer, typelist_1( rt_data )>
    {
        public:
            rt_monarch_consumer();
            virtual ~rt_monarch_consumer();

        public:
            void set_file( const string& p_filename );
            const string& get_file() const;

            void set_bits( const count_t& p_bits );
            const count_t& get_bits() const;

            void set_length( const count_t& p_length );
            const count_t& get_length() const;

            void set_duration( const real_t& p_duration);
            const real_t& get_duration() const;

            void set_voltage_minimum( const real_t& p_voltage_minimum );
            const real_t& get_voltage_minimum() const;

            void set_voltage_maximum( const real_t& p_voltage_maximum );
            const real_t& get_voltage_maximum() const;

        private:
            string f_file;
            count_t f_bits;
            count_t f_length;
            real_t f_duration;
            real_t f_voltage_minimum;
            real_t f_voltage_maximum;
            real_t f_voltage_range;
            real_t f_voltage_inverse_range;
            real_t f_voltage_levels;
            real_t f_voltage_inverse_levels;

            count_t f_size;
            real_t f_interval;
            const real_t* f_in;
            count_t f_index;
            count_t f_samples;
            count_t f_records;

            monarch::Monarch* f_monarch;
            monarch::MonarchHeader* f_header;
            monarch::MonarchRecord< monarch::DataType >* f_record;

        protected:
            void initialize_consumer();
            bool start_consumer();
            bool execute_consumer();
            bool stop_consumer();
            void finalize_consumer();
    };

}

#endif
