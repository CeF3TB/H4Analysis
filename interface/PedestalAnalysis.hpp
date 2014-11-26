#ifndef PEDESTAL_ANALYSIS_H
#define PEDESTAL_ANALYSIS_H

#include "interface/BaseAnalysis.hpp"
#include "interface/LooperAndFiller.hpp"

class PedestalAnalysis : public BaseAnalysis
{
/* Produce the profile histograms to do the subtraction
 *
 */

public:
	PedestalAnalysis(){ muMin=-2; muMax=6;muDelta=0.2; nCat=3;};
	~PedestalAnalysis(){};
	int nChannels;
	//vector<int> HV;
	float muMin;
	float muMax;
	float muDelta;
	int nCat;
	void Init(LoopAndFill *l1); // { l = l1 ; };
	void AnalyzeEvent();
	void ClearEvent();
	
	// regression stuff
	static float mean(vector<float> &a);
	static pair<float,float> regression(vector<float>&a,vector<float>&b);
	static char computeCategory(const UInt_t digiChannel,const float dmu2,const float dum1);
	
	
};

#endif

