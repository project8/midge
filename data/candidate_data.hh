#ifndef _midge_candidate_data_hh_
#define _midge_candidate_data_hh_

#include "types.hh"
#include "fourier.hh"
#include "binary.hh"
#include "ascii.hh"
#include "_stream.hh"
#include "macros.hh"

#include "coremsg.hh"

namespace midge
{

    class candidate
    {
        public:
            candidate();
            ~candidate();

        public:
            ;referrable( real_t, candidate_score )
            ;referrable( real_t, cluster_score )
            ;referrable( real_t, line_score )

    };

    class candidate_data :
        public _stream< candidate_data >
    {
        public:
            candidate_data();
            virtual ~candidate_data();

        public:
            command_t command();
            void command( command_t );
            candidate_data& operator>>( candidate_data& p_data );
            candidate_data& operator<<( const candidate_data& p_data );

        public:
            ;accessible( real_t, time_interval )
            ;accessible( count_t, time_index )
            ;accessible( real_t, frequency_interval )
            ;accessible( count_t, frequency_index )

        public:
            const candidate& operator[]( const count_t& p_index ) const;
            candidate& operator[]( const count_t& p_index );

            void set_size( const count_t& p_size );
            const count_t& get_size() const;

        private:
            typedef vector< candidate > data_t;
            typedef data_t::iterator it_t;
            typedef data_t::const_iterator cit_t;

            data_t f_data;
            count_t f_size;
    };

}

#endif
