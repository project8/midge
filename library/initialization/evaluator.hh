#ifndef _midge_evaluator_hh_
#define _midge_evaluator_hh_

#include "../library/initialization/arguments.hh"
#include "../library/initialization/processor.hh"

namespace midge
{

    class evaluator :
        public processor
    {
        public:
            evaluator( const arguments& p_arguments );
            virtual ~evaluator();

        public:
            virtual void process_lingual( string p_string );

        private:
            string evaluate( const string& p_string );

            const arguments& f_arguments;
            static const string s_start_bracket;
            static const string s_end_bracket;
    };

}

#endif
