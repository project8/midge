#ifndef _midge_ctf_cf_average_transformer_hh_
#define _midge_ctf_cf_average_transformer_hh_

#include "_average_transformer.hh"
#include "ctf_data.hh"
#include "cf_data.hh"

namespace midge
{

    typedef _average_transformer< ctf_data, cf_data > ctf_cf_average_transformer;

}

#endif
