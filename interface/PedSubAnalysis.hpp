#ifndef PEDSUB_ANALYSIS_H
#define PEDSUB_ANALYSIS_H

#include "interface/BaseAnalysis.hpp"
#include "interface/LooperAndFiller.hpp"

class PedSubAnalysis : public BaseAnalysis
{
/* Produce the profile histograms to do the subtraction
 *
 */
TFile *pedFile;
map<int,TProfile*> pedHisto; // ch
//map<string,TProfile*> pedHisto; 
public:
	PedSubAnalysis(){pedFile=NULL;nChannels=4;};
	~PedSubAnalysis(){Close();};
	int nChannels;
	//vector<int> HV;
	void Init(LoopAndFill *l1); // { l = l1 ; };
	void ClearEvent();
	void AnalyzeEvent();
	void Close();
	string pedestalFileName;
};

#endif

