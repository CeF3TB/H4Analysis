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
	PedestalAnalysis(){pedMin=3550; pedMax=3610;pedDelta=0.2;};
	~PedestalAnalysis(){};
	int nChannels;
	vector<int> HV;
	int pedMin;
	int pedMax;
	float pedDelta;
	void Init(LoopAndFill *l1); // { l = l1 ; };
	void AnalyzeEvent();
	void ClearEvent();
	
	// regression stuff
	static float mean(vector<float> &a);
	static pair<float,float> regression(vector<float>&a,vector<float>&b);
	
	
};

#endif

