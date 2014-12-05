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
	PedestalAnalysis(){};
	~PedestalAnalysis(){};
	int nChannels;
	//vector<int> HV;
	void Init(LoopAndFill *l1); // { l = l1 ; };
	void AnalyzeEvent();
	void ClearEvent();
	
};

#endif

