#include "midge.hh"
#include "rt_harmonic_producer.hh"
#include "rt_plot_consumer.hh"
#include "rt_rtf_power_transformer.hh"
#include "rtf_plot_consumer.hh"
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
    t_file.open( "test_harmonic_producer.log.txt", std::ios_base::trunc );

    messages* t_messages = messages::get_instance();
    t_messages->set_terminal_severity( s_debug );
    t_messages->set_terminal_stream( &cout );
    t_messages->set_log_severity( s_debug );
    t_messages->set_log_stream( &t_file );

    ::midge::midge* t_midge = new ::midge::midge();

    rt_harmonic_producer* t_rt_in = new rt_harmonic_producer();
    t_rt_in->set_name( "rt_in" );
    t_rt_in->set_impedance_ohm( 50. );
    t_rt_in->set_power_dbm( -10. );
    t_rt_in->set_frequency_hz( 100.e6 );
    t_rt_in->set_phase_deg( 60. );
    t_rt_in->set_begin_sec( 0.e-9 );
    t_rt_in->set_end_sec( 20000.e-9 );
    t_rt_in->set_start_sec( 2000.e-9 );
    t_rt_in->set_stop_sec( 18000.e-9 );
    t_rt_in->set_interval_sec( 1.e-9 );
    t_rt_in->set_length( 100 );
    t_rt_in->set_size( 1000 );
    t_rt_in->set_stride( 10 );
    t_midge->add( t_rt_in );

    rt_plot_consumer* t_rt_plot = new rt_plot_consumer();
    t_rt_plot->set_name( "rt_plot" );
    t_rt_plot->set_plot_key( "rt_plot" );
    t_rt_plot->set_plot_name( "rt_plot" );
    t_rt_plot->set_chart_title( "Harmonic Producer Signal" );
    t_rt_plot->set_x_title( "Time [sec]" );
    t_rt_plot->set_y_title( "Signal [volt]" );
    t_midge->add( t_rt_plot );

    rt_rtf_power_transformer* t_rt_rtf = new rt_rtf_power_transformer();
    t_rt_rtf->set_name( "rt_rtf" );
    t_rt_rtf->set_impedance_ohm( 50. );
    t_rt_rtf->set_window( new window_rectangular() );
    t_rt_rtf->set_length( 100 );
    t_midge->add( t_rt_rtf );

    rtf_plot_consumer* t_rtf_plot = new rtf_plot_consumer();
    t_rtf_plot->set_name( "rtf_plot" );
    t_rtf_plot->set_plot_key( "rtf_plot" );
    t_rtf_plot->set_plot_name( "rtf_plot" );
    t_rtf_plot->set_chart_title( "Harmonic Producer Spectrum" );
    t_rtf_plot->set_x_title( "Time [sec]" );
    t_rtf_plot->set_y_title( "Frequency [hertz]" );
    t_rtf_plot->set_z_title( "Power [watt]" );
    t_midge->add( t_rtf_plot );

    t_midge->join( "rt_in.out_0:rt_plot.in_0" );
    t_midge->join( "rt_in.out_0:rt_rtf.in_0" );
    t_midge->join( "rt_rtf.out_0:rtf_plot.in_0" );

    t_midge->run( "rt_in:rt_plot:rt_rtf:rtf_plot" );

    delete t_midge;

    t_file.close();

    return 0;
}
