#ifndef PLOT_ANALYSIS_H
#define PLOT_ANALYSIS_H

#include "interface/BaseAnalysis.hpp"
#include "interface/LooperAndFiller.hpp"

class PlotAnalysis : public BaseAnalysis
{
/* Produce the profile histograms to do the subtraction for Corr1. 
 * This histograms must be fitted with test/fit.C to produce the exact files that are used
 */
public:
	PlotAnalysis(){};
	~PlotAnalysis(){};
	int nChannels;
	//vector<int> HV;
	vector<int> E;
	void Init(LoopAndFill *l1); // { l = l1 ; };
	void AnalyzeEvent();
	// make the plots only for these runs. -- alternatively table position?
	map<int,int> runs; 
	bool isActiveRun(int run); // if empty all
	void AddRun(int run){runs[run]=1;};
	void AddRunRange(int run1,int run2){for(int i=run1;i<=run2;++i) AddRun(i);};
	
	vector<float> fwhmBins;
	void AddRangeFwhm(float min,float max, float d){for(float x=min;x<=max ; x+=d)fwhmBins.push_back(x);}
};

#endif

