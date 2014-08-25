#include "root.hh"
#include "rt_sinusoidal_producer.hh"
#include "rt_ascii_consumer.hh"
#include "rt_ct_transformer.hh"
#include "ct_ctf_fourier_transformer.hh"
#include "ctf_rtf_transformer.hh"
#include "rtf_ascii_consumer.hh"
#include "window_rectangular.hh"
using namespace midge;

#include <fstream>
using std::ofstream;

#include <iostream>
using std::cout;
using std::endl;

#include <cmath>

int main()
{
    ofstream t_file;
    t_file.open( "test_fourier_transformer.log.txt", std::ios_base::trunc );

    messages* t_messages = messages::get_instance();
    t_messages->set_terminal_severity( s_debug );
    t_messages->set_terminal_stream( &cout );
    t_messages->set_log_severity( s_debug );
    t_messages->set_log_stream( &t_file );

    midge* t_root = new midge();

    rt_sinusoidal_producer* t_rt_in = new rt_sinusoidal_producer();
    t_rt_in->set_name( "rt_in" );
    t_rt_in->set_impedance_ohm( 50. );
    t_rt_in->set_carrier_power_dbm( -10. );
    t_rt_in->set_carrier_frequency_hz( 250.e6 );
    t_rt_in->set_carrier_phase_deg( 60. );
    t_rt_in->set_signal_amplitude_hz( 50.e6 );
    t_rt_in->set_signal_frequency_hz( 500.e3 );
    t_rt_in->set_signal_phase_deg( 180. );
    t_rt_in->set_begin_sec( 0.e-9 );
    t_rt_in->set_end_sec( 20000.e-9 );
    t_rt_in->set_start_sec( 2000.e-9 );
    t_rt_in->set_stop_sec( 18000.e-9 );
    t_rt_in->set_interval_sec( 1.e-9 );
    t_rt_in->set_length( 100 );
    t_rt_in->set_size( 1000 );
    t_rt_in->set_stride( 10 );
    t_root->add( t_rt_in );

    rt_ascii_consumer* t_rt_out = new rt_ascii_consumer();
    t_rt_out->set_name( "rt_out" );
    t_rt_out->set_file( "test_fourier_transformer.input.txt" );
    t_root->add( t_rt_out );

    rt_ct_transformer* t_rt_ct = new rt_ct_transformer();
    t_rt_ct->set_name( "rt_ct" );
    t_rt_ct->set_mode( "real" );
    t_rt_ct->set_length( 100 );
    t_root->add( t_rt_ct );

    ct_ctf_fourier_transformer* t_ct_cf = new ct_ctf_fourier_transformer();
    t_ct_cf->set_name( "ct_cf" );
    t_ct_cf->set_length( 100 );
    t_root->add( t_ct_cf );

    ctf_rtf_transformer* t_cf_rf_re = new ctf_rtf_transformer();
    t_cf_rf_re->set_name( "cf_rf_re" );
    t_cf_rf_re->set_mode( "real" );
    t_cf_rf_re->set_length( 100 );
    t_root->add( t_cf_rf_re );

    rtf_ascii_consumer* t_rf_re_out = new rtf_ascii_consumer();
    t_rf_re_out->set_name( "rf_re_out" );
    t_rf_re_out->set_file( "test_fourier_transformer.re.txt" );
    t_root->add( t_rf_re_out );

    ctf_rtf_transformer* t_cf_rf_im = new ctf_rtf_transformer();
    t_cf_rf_im->set_name( "cf_rf_im" );
    t_cf_rf_im->set_mode( "imaginary" );
    t_cf_rf_im->set_length( 100 );
    t_root->add( t_cf_rf_im );

    rtf_ascii_consumer* t_rf_im_out = new rtf_ascii_consumer();
    t_rf_im_out->set_name( "rf_im_out" );
    t_rf_im_out->set_file( "test_fourier_transformer.im.txt" );
    t_root->add( t_rf_im_out );

    t_root->join( "rt_in.out_0:rt_out.in_0" );
    t_root->join( "rt_in.out_0:rt_ct.in_0" );
    t_root->join( "rt_ct.out_0:ct_cf.in_0" );
    t_root->join( "ct_cf.out_0:cf_rf_re.in_0" );
    t_root->join( "cf_rf_re.out_0:rf_re_out.in_0" );
    t_root->join( "ct_cf.out_0:cf_rf_im.in_0" );
    t_root->join( "cf_rf_im.out_0:rf_im_out.in_0" );

    t_root->run( "rt_in:rt_out:rt_ct:ct_cf:cf_rf_re:rf_re_out:cf_rf_im:rf_im_out" );

    delete t_root;

    t_file.close();

    return 0;
}
