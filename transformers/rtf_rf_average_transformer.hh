#ifndef _midge_rtf_rf_average_transformer_hh_
#define _midge_rtf_rf_average_transformer_hh_

#include "_average_transformer.hh"
#include "rtf_data.hh"
#include "rf_data.hh"

namespace midge
{

    typedef _average_transformer< rtf_data, rf_data > rtf_rf_average_transformer;

}

#endif
