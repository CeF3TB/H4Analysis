#ifndef EVENT_H
#define EVENT_H

#include "TTree.h"
#include <map>
#include <string>
using namespace std;

 #define MAX_ADC_CHANNELS 200
 #define MAX_DIGI_SAMPLES 100000
 #define MAX_TDC_CHANNELS 200
 #define MAX_SCALER_WORDS 16
 #define MAX_PATTERNS 16
 #define MAX_PATTERNS_SHODO 16
 #define SMALL_HODO_X_NFIBERS 8
 #define SMALL_HODO_Y_NFIBERS 8
 #define MAX_TRIG_WORDS 32
 #define MAX_RO 10


class tree
{
public:
map<string,bool> isFilled;
	// ---- COMMON ---
	UInt_t runNumber;
	UInt_t spillNumber;
	UInt_t evtNumber;
	// ---- H4tree ---
	UInt_t evtTimeDist;
	UInt_t evtTimeStart;
	UInt_t nEvtTimes;
	ULong64_t evtTime[MAX_RO];
	UInt_t evtTimeBoard[MAX_RO];
	UInt_t nAdcChannels;
	UInt_t adcBoard[MAX_ADC_CHANNELS];
	UInt_t adcChannel[MAX_ADC_CHANNELS];
	UInt_t adcData[MAX_ADC_CHANNELS];
	UInt_t nTdcChannels;
	UInt_t tdcBoard[MAX_TDC_CHANNELS];
	UInt_t tdcChannel[MAX_TDC_CHANNELS];
	UInt_t tdcData[MAX_TDC_CHANNELS];
	UInt_t nDigiSamples;
	UInt_t digiFrequency[MAX_DIGI_SAMPLES];
	UInt_t digiGroup[MAX_DIGI_SAMPLES];
	UInt_t digiChannel[MAX_DIGI_SAMPLES];
	UInt_t digiSampleIndex[MAX_DIGI_SAMPLES];
	Float_t digiSampleValue[MAX_DIGI_SAMPLES];
	Float_t digiSampleValueSub[MAX_DIGI_SAMPLES];
	UInt_t digiBoard[MAX_DIGI_SAMPLES];
	UInt_t nScalerWords;
	UInt_t scalerWord[MAX_SCALER_WORDS];
	UInt_t scalerBoard[MAX_SCALER_WORDS];
	UInt_t nPatterns;
	UInt_t pattern[MAX_PATTERNS];
	UInt_t patternBoard[MAX_PATTERNS];
	UInt_t patternChannel[MAX_PATTERNS];
	UInt_t nTriggerWords;
	UInt_t triggerWords[MAX_TRIG_WORDS];
	UInt_t triggerWordsBoard[MAX_TRIG_WORDS];
	// ---- dqmTree ----
 	vector<float>  *ADCvalues ;
 	vector<float>  *BGOvalues ;
 	vector<float>  *SCINTvalues ;
 	vector<float>  *TDCreco   ;
 	vector<float>  *digi_charge_integrated ;
 	vector<float>  *digi_charge_integrated_frac50 ;
 	vector<float>  *digi_charge_integrated_frac30 ;
 	vector<float>  *digi_charge_integrated_frac10 ;
 	vector<float>  *digi_max_amplitude ;
 	vector<float>  *digi_pedestal ;
 	vector<float>  *digi_pedestal_rms ;
 	vector<float>  *digi_time_at_frac30 ;
 	vector<float>  *digi_time_at_frac50 ;
 	vector<float>  *digi_fall_time_at_frac30 ;
 	vector<float>  *digi_fall_time_at_frac50 ;
 	vector<float>  *digi_time_at_max ;
	// --- no fft
 	vector<float>  *digi_max_amplitude_bare ;
 	vector<float>  *digi_time_at_max_bare ;
 	vector<float>  *digi_charge_integrated_bare ;
 	vector<float>  *digi_time_at_frac30_bare ;
 	vector<float>  *digi_time_at_frac50_bare ;
 	vector<float>  *digi_fall_time_at_frac30_bare ;
 	vector<float>  *digi_fall_time_at_frac50_bare ;
	// --- sub
 	vector<float>  *digi_charge_integrated_sub ;
 	vector<float>  *digi_max_amplitude_sub ;
 	vector<float>  *digi_pedestal_sub ;
 	vector<float>  *digi_pedestal_rms_sub ;

 	vector<float>  *digi_charge_integrated_corr1 ;
 	vector<float>  *digi_max_amplitude_corr1 ;
	
 	vector<float>  *digi_charge_integrated_corr2 ;
 	vector<float>  *digi_max_amplitude_corr2 ;
	
	// --- FFT
 	vector<float >  *digi_value ;
 	vector<int >    *digi_value_ch ;
 	vector<float >  *digi_value_time ;

	// ---- HODO

 	vector<bool>   *HODOX1    ;
 	vector<bool>   *HODOX2    ;
 	vector<bool>   *HODOY1    ;
 	vector<bool>   *HODOY2    ;
	// -- DEBUG
	
 	vector<float>  *digi_pedestal_m ; 
 	vector<float>  *digi_pedestal_q ;

 	vector<float>  *digi_pedestal_m2 ; 
 	vector<float>  *digi_pedestal_q2 ;
 	vector<float>  *digi_pedestal_mu2 ; 

 	vector<float>  *digi_pedestal_m1 ; 
 	vector<float>  *digi_pedestal_q1 ;
 	vector<float>  *digi_pedestal_mu1 ; 

 	vector<float>  *digi_pedestal_m0 ; 
 	vector<float>  *digi_pedestal_q0 ;
 	vector<float>  *digi_pedestal_mu0 ; 

  float TableX;
  float TableY;
  float CeF3HV;
  float BGOHV;
  float BeamEnergy;
  float BeamTilt;
  int IsPhysics;

  vector<int> *nTdcHits;

	tree();
	~tree();
	
};
#endif
