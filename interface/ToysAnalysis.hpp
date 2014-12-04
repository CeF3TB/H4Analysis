#ifndef TOYS_ANALYSIS_H
#define TOYS_ANALYSIS_H

#include "interface/BaseAnalysis.hpp"

#include "TRandom.h"
#include "TRandom3.h"
#include "TF1.h"
#include "TH1D.h"

class ToysAnalysis: public BaseAnalysis
{
private:
	TRandom3 *r;
	int iEvent;
	TF1 *fEvent; 
	float fIntegral;
	TH1D *hEvent;

public:
// -- Constructor
	ToysAnalysis() : BaseAnalysis(){
		//private
		iEvent=0;
		//public
		resolution=0.1;
		seed=-1;
		noise_rms=0.1;
		charge_mean=1;
		x_shift_rms=5; // digiSampleIndex
		nChannels=1;
		};
// -- Destructor
	~ToysAnalysis(){};
// -- Analysis member functions
	void Init(LoopAndFill *l1); // { l = l1 ; };
	void AnalyzeEvent();
	void ClearEvent();
// -- public configurable members
	float resolution;
	float noise_rms;
	long int seed;
	float charge_mean; // mean energy of the charge integration
	float x_shift_rms;
	int nChannels;

};

#endif
