#include "interface/Event.hpp"


tree::tree(){
	ADCvalues =new              vector<float> ; 
	BGOvalues =new              vector<float> ; 
	SCINTvalues =new            vector<float> ; 
	TDCreco   =new              vector<float> ; 
	digi_charge_integrated =new vector<float> ; 
	digi_charge_integrated_frac50 =new vector<float> ; 
	digi_charge_integrated_frac30 =new vector<float> ; 
	digi_charge_integrated_frac10 =new vector<float> ; 
	digi_max_amplitude =new     vector<float> ; 
	digi_pedestal =new          vector<float> ; 
	digi_pedestal_rms =new      vector<float> ; 
	digi_time_at_frac30 =new    vector<float> ; 
	digi_time_at_frac50 =new    vector<float> ; 
	digi_fall_time_at_frac30 =new    vector<float> ; 
	digi_fall_time_at_frac50 =new    vector<float> ; 
	digi_time_at_max =new       vector<float> ; 
	HODOX1    =new              vector<bool>  ; 
	HODOX2    =new              vector<bool>  ; 
	HODOY1    =new              vector<bool>  ; 
	HODOY2    =new              vector<bool>  ; 

	digi_charge_integrated_sub =new vector<float> ; 
	digi_max_amplitude_sub =new     vector<float> ; 
	digi_pedestal_sub =new          vector<float> ; 
	digi_pedestal_rms_sub =new      vector<float> ; 

	digi_charge_integrated_corr1 =new vector<float> ; 
	digi_max_amplitude_corr1 =new     vector<float> ; 

	digi_charge_integrated_corr2 =new vector<float> ; 
	digi_max_amplitude_corr2 =new     vector<float> ; 
	// --- no fft
 	digi_max_amplitude_bare  = new vector<float>;
 	digi_time_at_max_bare  = new vector<float>;
 	digi_charge_integrated_bare  = new vector<float>;
 	digi_time_at_frac30_bare  = new vector<float>;
 	digi_time_at_frac50_bare  = new vector<float>;
 	digi_fall_time_at_frac30_bare  = new vector<float>;
 	digi_fall_time_at_frac50_bare  = new vector<float>;

	// -- DEBUG
	digi_pedestal_m =new vector<float> ; 
	digi_pedestal_q =new vector<float> ; 

	digi_pedestal_m0 =new vector<float> ; 
	digi_pedestal_q0 =new vector<float> ; 
	digi_pedestal_m1 =new vector<float> ; 
	digi_pedestal_q1 =new vector<float> ; 
	digi_pedestal_m2 =new vector<float> ; 
	digi_pedestal_q2 =new vector<float> ; 
	digi_pedestal_mu0 =new vector<float> ; 
	digi_pedestal_mu1 =new vector<float> ; 
	digi_pedestal_mu2 =new vector<float> ; 
	
	// --- FFT
	digi_value 	= new vector<float>;
	digi_value_ch 	= new vector<int>;
	digi_value_time = new vector<float>;
	// ----

	nTdcHits = new vector<int>;

}

tree::~tree(){
	delete ADCvalues ;
	delete BGOvalues ;
	delete SCINTvalues ;
	delete TDCreco   ;
	delete digi_charge_integrated ;
	delete digi_max_amplitude ;
	delete digi_pedestal ;
	delete digi_pedestal_rms ;
	delete digi_time_at_frac30 ;
	delete digi_time_at_frac50 ;
	delete digi_fall_time_at_frac30 ;
	delete digi_fall_time_at_frac50 ;
	delete digi_time_at_max ;
	delete HODOX1    ;
	delete HODOX2    ;
	delete HODOY1    ;
	delete HODOY2    ;

	delete digi_charge_integrated_sub ;
	delete digi_max_amplitude_sub ;
	delete digi_pedestal_sub ;
	delete digi_pedestal_rms_sub ;

	delete digi_charge_integrated_corr1 ;
	delete digi_max_amplitude_corr1 ;

	delete digi_charge_integrated_corr2 ;
	delete digi_max_amplitude_corr2 ;

	// DEBUG	
	delete digi_pedestal_m;
	delete digi_pedestal_q;

	delete nTdcHits;

}
