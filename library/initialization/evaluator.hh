#ifndef _midge_evaluator_hh_
#define _midge_evaluator_hh_

#include "arguments.hh"
#include "processor.hh"

namespace midge
{

    class evaluator :
        public processor
    {
        public:
            evaluator( const arguments& p_arguments );
            virtual ~evaluator();

        public:
            virtual void process_lingual( std::string p_string );

        private:
            std::string evaluate( const std::string& p_string );

            const arguments& f_arguments;
            static const std::string s_start_bracket;
            static const std::string s_end_bracket;
    };

}

#endif
