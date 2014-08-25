#include "root.hh"
#include "rt_harmonic_producer.hh"
#include "rt_ct_analytic_transformer.hh"
#include "ct_rt_transformer.hh"
#include "rt_ascii_consumer.hh"
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
    t_file.open( "test_harmonic_producer.log.txt", std::ios_base::trunc );

    messages* t_messages = messages::get_instance();
    t_messages->set_terminal_severity( s_debug );
    t_messages->set_terminal_stream( &cout );
    t_messages->set_log_severity( s_debug );
    t_messages->set_log_stream( &t_file );

    midge* t_root = new midge();

    rt_harmonic_producer* t_rt_in = new rt_harmonic_producer();
    t_rt_in->set_name( "rt_in" );
    t_rt_in->set_impedance_ohm( 50. );
    t_rt_in->set_power_dbm( -10. );
    t_rt_in->set_frequency_hz( 2.e6 );
    t_rt_in->set_phase_deg( 60. );
    t_rt_in->set_begin_sec( 0.e-9 );
    t_rt_in->set_end_sec( 20000.e-9 );
    t_rt_in->set_start_sec( 2000.e-9 );
    t_rt_in->set_stop_sec( 18000.e-9 );
    t_rt_in->set_interval_sec( 1.e-9 );
    t_rt_in->set_length( 10 );
    t_rt_in->set_size( 20000 );
    t_rt_in->set_stride( 20000 );
    t_root->add( t_rt_in );

    rt_ascii_consumer* t_rt_out = new rt_ascii_consumer();
    t_rt_out->set_name( "rt_out" );
    t_rt_out->set_file( "test_analytic_transformer.input.txt" );
    t_root->add( t_rt_out );

    rt_ct_analytic_transformer* t_rt_ct = new rt_ct_analytic_transformer();
    t_rt_ct->set_name( "rt_ct" );
    t_rt_ct->set_length( 10 );
    t_root->add( t_rt_ct );

    ct_rt_transformer* t_ct_rt_re = new ct_rt_transformer();
    t_ct_rt_re->set_name( "ct_rt_re" );
    t_ct_rt_re->set_mode( "real" );
    t_ct_rt_re->set_length( 10 );
    t_root->add( t_ct_rt_re );

    ct_rt_transformer* t_ct_rt_im = new ct_rt_transformer();
    t_ct_rt_im->set_name( "ct_rt_im" );
    t_ct_rt_im->set_mode( "imaginary" );
    t_ct_rt_im->set_length( 10 );
    t_root->add( t_ct_rt_im );

    rt_ascii_consumer* t_rt_re_out = new rt_ascii_consumer();
    t_rt_re_out->set_name( "rt_re_out" );
    t_rt_re_out->set_file( "test_analytic_transformer.re.txt" );
    t_root->add( t_rt_re_out );

    rt_ascii_consumer* t_rt_im_out = new rt_ascii_consumer();
    t_rt_im_out->set_name( "rt_im_out" );
    t_rt_im_out->set_file( "test_analytic_transformer.im.txt" );
    t_root->add( t_rt_im_out );

    t_root->join( "rt_in.out_0:rt_out.in_0" );
    t_root->join( "rt_in.out_0:rt_ct.in_0" );
    t_root->join( "rt_ct.out_0:ct_rt_re.in_0" );
    t_root->join( "ct_rt_re.out_0:rt_re_out.in_0" );
    t_root->join( "rt_ct.out_0:ct_rt_im.in_0" );
    t_root->join( "ct_rt_im.out_0:rt_im_out.in_0" );

    t_root->run( "rt_in:rt_out:rt_ct:ct_rt_re:rt_re_out:ct_rt_im:rt_im_out" );

    delete t_root;

    t_file.close();

    return 0;
}
