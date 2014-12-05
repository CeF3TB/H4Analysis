#ifndef FIT_ANALYSIS_H
#define FIT_ANALYSIS_H

#include "interface/BaseAnalysis.hpp"
#include "interface/WaveformFit.hpp"

#include "TProfile.h"
#include <map>
#include <vector>
using namespace std;

class FitAnalysis : public BaseAnalysis {
/*
 * Fill the _fit branches after a fit eventually performes some subtractions
 * fit is based on profiles
 */
protected:
	map<pair<int,int>,TProfile*> histos; // associated with fwhmBins: index,ch
	ROOT::Math::Minimizer *minimizer;
public:
	FitAnalysis();
	~FitAnalysis();
	virtual void Init(LoopAndFill*l1); 
	virtual void AnalyzeEvent();
	virtual void ClearEvent(); // clear  all the output it is going to fill

	// configuration
	string plotFileName;
	vector<float> fwhmBins;
	int nChannels;
        void AddRangeFwhm(float min,float max, float d){for(float x=min;x<=max ; x+=d)fwhmBins.push_back(x);}

};


#endif
